/*
 * Usage:
 *   sudo insmod va_space.ko pid=<PID>
 *   dmesg | tail
 *   sudo rmmod va_space
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>      //  task_struct, for_each_process
#include <linux/mm.h>         //mm_struct, vm_area_struct
#include <linux/mm_types.h>   // mm_struct definition
#include <linux/pid.h>        // find_get_pid, pid_task
#include <linux/maple_tree.h> // maple tree VMA traversal
// modlue license and header for kernal access
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abe Anderson");
MODULE_DESCRIPTION("reports total virtual address space size of a process");
// base module parameter: process ID
static int pid = 1;
module_param(pid, int, 0444);
MODULE_PARM_DESC(pid, "PID of the target process");
// module init
static int __init va_space_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    unsigned long total_size = 0;
    unsigned long vma_count = 0;
    unsigned long index = 0; /* maple tree iteration index */

    printk(KERN_INFO "va_space: Module loaded by process '%s' (PID %d)\n", current->comm, current->pid);
    printk(KERN_INFO "va_space: Looking up target PID %d\n", pid);
    // lookup task_struct for given PID
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    //
    if (!task)
    {
        printk(KERN_ERR "va_space: Cannot find task for PID %d\n", pid);
        return -ESRCH;
    }
    printk(KERN_INFO "va_space: Found process '%s' (PID %d)\n",
           task->comm, task->pid);

    // set mem descriptor
    mm = task->mm;
    if (!mm)
    {
        printk(KERN_ERR "va_space: PID %d is a kernel thread (no mm)\n", pid);
        return -EINVAL;
    }

    /*
     * traverse VMAs using the maple tree interface
     *
     * mt_for_each() iterates over all entries in the maple tree.
     * mm->mm_mt is the maple tree storing all VMAs.
     * vma is set to each vm_area_struct* in turn.
     * index is updated to the current key or start address.
     * ULONG_MAX is the upper bound .
     */
    mt_for_each(&mm->mm_mt, vma, index, ULONG_MAX)
    {
        unsigned long size = vma->vm_end - vma->vm_start;
        total_size += size;
        vma_count++;

        printk(KERN_DEBUG "va_space: VMA #%lu: 0x%lx - 0x%lx  size=%lu bytes\n",
               vma_count, vma->vm_start, vma->vm_end, size);
    }
    printk(KERN_INFO "va_space: Process '%s' (PID %d)\n", task->comm, pid);
    printk(KERN_INFO "va_space:   Number of VMAs : %lu\n", vma_count);
    printk(KERN_INFO "va_space:   Total VA space  : %lu bytes\n", total_size);
    printk(KERN_INFO "va_space:   Total VA space  : %lu kB\n", total_size / 1024);
    printk(KERN_INFO "va_space: (Compare with: sudo pmap %d | tail -1)\n", pid);
    return 0;
}
// exit module func
static void __exit va_space_exit(void)
{
    printk(KERN_INFO "va_space: Module unloaded by process '%s' (PID %d)\n",
           current->comm, current->pid);
}
/*module call and exit*/
module_init(va_space_init);
module_exit(va_space_exit);