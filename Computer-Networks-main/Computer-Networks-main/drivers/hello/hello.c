#include<linux/module.h>    //needed for module_init and module_exit
#include<linux/kernel.h>    //needed for KERN_INFO
#include<linux/init.h>      //needed for the macros

int __init hw_init(void) {
        printk(KERN_INFO"Hello World\n");
        return 0;
}

void __exit hw_exit(void) {
        printk(KERN_INFO"Bye World\n");
}

MODULE_LICENSE("GPL");

module_init(hw_init);
module_exit(hw_exit);
