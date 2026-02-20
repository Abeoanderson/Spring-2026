#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>   // for task_struct and parent links
#include <linux/pid.h>            // for find_get_pid and pid_task

// Module parameter
static int target_pid = 1;
module_param(target_pid, int, 0);
MODULE_PARM_DESC(target_pid, "PID of the process to inspect");

// Convert numeric state to readable text (simple version)
static const char *get_state(long state)
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

    printk(KERN_INFO "Inspector module loaded for PID: %d\n", target_pid);

    // Convert PID → task_struct
    pid_struct = find_get_pid(target_pid);
    task = pid_task(pid_struct, PIDTYPE_PID);

    if (!task) {
        printk(KERN_INFO "Inspector: PID %d not found.\n", target_pid);
        return -EINVAL;
    }

    printk(KERN_INFO "Process ancestry:\n");

    // Walk up the family tree until PID 1
    while (task) {
        printk(KERN_INFO "PID: %d | Name: %s | State: %s\n", task->pid, task->comm, get_state(task->state));

        if (task->pid == 1)
            break;

        task = task->real_parent;
    }

    return 0;
}

static void __exit inspector_exit(void)
{
    printk(KERN_INFO "Inspector module unloaded.\n");
}

module_init(inspector_init);
module_exit(inspector_exit);

