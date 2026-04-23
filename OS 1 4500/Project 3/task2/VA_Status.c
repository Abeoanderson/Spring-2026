/*
 * Usage:
 *   sudo insmod va_status.ko pid=<PID> vaddr=<HEX_ADDRESS>
 *   dmesg | tail
 *   sudo rmmod va_status
 *
 * The virtual address is passed as a hex string without using "0x" prefix for ease of testing
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>      // task_struct
#include <linux/mm.h>         // mm_struct, page table macros
#include <linux/mm_types.h>   // mm_struct, page
#include <linux/pid.h>        // find_get_pid, pid_task
#include <linux/pgtable.h>    // pgd_offset, p4d_offset, pud_offset, pmd_offset, pte_offset_kernel
#include <linux/page-flags.h> // PageSwapCache, etc.
// module license and header for kernal access
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abe Anderson");
MODULE_DESCRIPTION("reports whether a virtual address is in memory or on disk");

// base module params, PID
static int pid = 1;
module_param(pid, int, 0444);
MODULE_PARM_DESC(pid, "PID of the target process");

// for vaddr theirs no sscanf for modules so I had to use charp and kstrtoul to convert
static char *vaddr = "0";
module_param(vaddr, charp, 0444);
MODULE_PARM_DESC(vaddr, "Virtual address to inspect (hex string, e.g., 7fff12340000)");
// module init
static int __init va_status_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;
    unsigned long addr;
    int ret;

    // 5 level page table
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;

    printk(KERN_INFO "va_status: Module loaded by process '%s' (PID %d)\n",
           current->comm, current->pid);

    // kstrtoul(string, base, result) returns 0 on success or negative and errrors on error
    ret = kstrtoul(vaddr, 16, &addr);
    if (ret) // on error print error
    {
        printk(KERN_ERR "va_status: Invalid virtual address string '%s' (err=%d)\n",
               vaddr, ret);
        return ret;
    }
    printk(KERN_INFO "va_status: Target PID=%d, virtual address=0x%lx (%lu)\n",
           pid, addr, addr);

    // lookup task_struct
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    if (!task) // if null  print error
    {
        printk(KERN_ERR "va_status: Cannot find task for PID %d\n", pid);
        return -ESRCH;
    }
    printk(KERN_INFO "va_status: Found process '%s' (PID %d)\n",
           task->comm, task->pid);
    mm = task->mm;
    if (!mm)
    {
        printk(KERN_ERR "va_status: PID %d is a kernel thread (no mm)\n", pid);
        return -EINVAL;
    }
    // 5 level page table walk
    // levl 1 is PGD - Page Global Directory
    // pgd_offset(mm, address) returns a pointer to the PGD entry that covers
    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        printk(KERN_INFO "va_status: Address 0x%lx - PGD not present/bad (address unmapped)\n", addr);
        return 0;
    }
    printk(KERN_DEBUG "va_status: PGD entry found at %px, value=0x%lx\n",
           pgd, pgd_val(*pgd));

    // level 2 is P4D - 4th level page Directory
    // p4d_offset(pgd, address) returns a pointer to the P4D entry.
    p4d = p4d_offset(pgd, addr);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
    {
        printk(KERN_INFO "va_status: Address 0x%lx - P4D not present/bad\n", addr);
        return 0;
    }
    printk(KERN_DEBUG "va_status: P4D entry found\n");
    // levl 3 is PUD - Page Upper Directory
    // pgd_offset(mm, address) returns a pointer to the PGD entry that covers
    /*
     * Level 3 - PUD (Page Upper Directory)
     * pgd_offset(mm, address) returns a pointer to the PGD entry that covers
     */
    pud = pud_offset(p4d, addr);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        printk(KERN_INFO "va_status: Address 0x%lx - PUD not present/bad\n", addr);
        return 0;
    }
    printk(KERN_DEBUG "va_status: PUD entry found\n");
    // levl 4 PMD - Page middle directory
    // pmd_offset(pud, address) returns a pointer to the PMD entry.
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
    {
        printk(KERN_INFO "va_status: Address 0x%lx - PMD not present/bad\n", addr);
        return 0;
    }
    printk(KERN_DEBUG "va_status: PMD entry found\n");
    // level 5 PTE - Page Table Entry
    // pte_offset_kernel(pmd, address) returns a pointer to the PTE.
    pte = pte_offset_kernel(pmd, addr);
    if (!pte)
    {
        printk(KERN_INFO "va_status: Address 0x%lx - PTE pointer is NULL\n", addr);
        return 0;
    }
    printk(KERN_DEBUG "va_status: PTE entry found at %px, value=0x%lx\n",
           pte, pte_val(*pte));

    // check presemt bit with pte_present()
    // pte_present(pte) checks the PRESENT bit in the PTE.
    if (pte_present(*pte)) // must derefrence pte cause pte_present takes a pte_t val not a pointer
    {
        printk(KERN_INFO "va_status: Address 0x%lx is IN MEMORY (PRESENT bit set)\n", addr);
        printk(KERN_INFO "va_status:   The physical page frame number (PFN): %lu\n",
               pte_pfn(*pte));
    }
    else // if present bit is 0, remaining bits of the PTE contian swap info, this inof shows swap device and slot offset on that device
    {
        printk(KERN_INFO "va_status: Address 0x%lx is ON DISK / SWAPPED OUT (PRESENT bit clear)\n", addr);
        printk(KERN_INFO "va_status:   PTE value: 0x%lx (encodes swap type + offset)\n",
               pte_val(*pte));
    }

    pte_unmap(pte); // release PTE maping
    return 0;
}
// module exit
static void __exit va_status_exit(void)
{
    printk(KERN_INFO "va_status: Module unloaded by process '%s' (PID %d)\n",
           current->comm, current->pid);
}
// module calls
module_init(va_status_init);
module_exit(va_status_exit);