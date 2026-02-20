#include <linux/module.h>
#include <linux/kernel.h>


int hello_init(void)
{
    printk(KERN_INFO "CS 4500 Project 2: Hello World loaded.\n");
    return 0;
}

// Runs when module is removed
void hello_exit(void)
{
    printk(KERN_INFO "CS 4500 Project 2: Goodbye.\n");
}

module_init(hello_init);
module_exit(hello_exit);
