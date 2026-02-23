#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>   // for task_struct and parent links
#include <linux/pid.h>            // for find_get_pid and pid_task

// Module parameter
static int target_pid = 1;
module_param(target_pid, int, 0);
MODULE_PARM_DESC(target_pid, "PID of the process to inspect");

static const char *get_state(unsigned int state)
{
    if (state == TASK_RUNNING)
        return "RUNNING";
    else if (state == TASK_INTERRUPTIBLE)
        return "SLEEPING";
    else if (state == TASK_UNINTERRUPTIBLE)
        return "UNINTERRUPTIBLE";
    else if (state == __TASK_STOPPED)
        return "STOPPED";
    else if (state == EXIT_ZOMBIE)
        return "ZOMBIE";
    else if (state == EXIT_DEAD)
        return "DEAD";
    else
        return "UNKNOWN";
}

static int __init inspector_init(void)
{
    struct pid *pid_struct;
    struct task_struct *task;
    int depth = 0;
    const int MAX_DEPTH = 100; // loop guard

    printk(KERN_INFO "Inspector module loaded for PID: %d\n", target_pid);

    // Convert PID with task_struct
    pid_struct = find_get_pid(target_pid);
    if (!pid_struct) {
        printk(KERN_INFO "Inspector: PID %d not found.\n", target_pid);
        return -EINVAL;
    }

    task = pid_task(pid_struct, PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "Inspector: Could not get task for PID %d.\n", target_pid);
        put_pid(pid_struct); // release reference before returning
        return -EINVAL;
    }

    printk(KERN_INFO "Process ancestry:\n");

    // Walk up the family tree until PID = 1 or MAX_DEPTH is hit
    while (task && depth < MAX_DEPTH) {
        printk(KERN_INFO "PID: %d | Name: %s | State: %s\n",
               task->pid,
               task->comm,
               get_state(task->__state));  // use __state instead of deprecated .state

        if (task->pid == 1)
            break;

        task = task->real_parent;
        depth++;
    }

    if (depth >= MAX_DEPTH)
        printk(KERN_WARNING "Inspector: hit max depth limit, ancestry walk stopped early.\n");

    // Release the reference from find_get_PID
    put_pid(pid_struct);

    return 0;
}

static void __exit inspector_exit(void)
{
    printk(KERN_INFO "Inspector module unloaded.\n");
}

module_init(inspector_init);
module_exit(inspector_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Inspector kernel module");
MODULE_AUTHOR("Abe Anderson");
