#include <linux/module.h>
#include <linux/kernel.h>

static int hello_init(void)
{
    printk(KERN_INFO "CS 4500 Project 2: Hello World loaded.\n");
    return 0;
}

// Runs when module is removed
static void hello_exit(void)
{
    printk(KERN_INFO "CS 4500 Project 2: Goodbye.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Kernal Module");
MODULE_AUTHOR("Abe Anderson");
