#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/stddef.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DINESH REDDY");
MODULE_DESCRIPTION("Capitalizing Device Driver");
static int __init initializer(void);
static void __exit exiter(void);
dev_t d=0;
static struct cdev mydev;
static struct class *device_class;
static int opendev(struct inode *inode,struct file *file);
static int closedev(struct inode *inode,struct file *file);
static ssize_t readcharacters(struct file *flip,char __user *buff,size_t len,loff_t *off);
static ssize_t writecharacters(struct file *flip,const char *buff,size_t len,loff_t *off);
static struct file_operations fops={
    .owner=THIS_MODULE,
    .read=readcharacters,
    .write=writecharacters,
    .open=opendev,
    .release=closedev
};
char *buffk;
static int opendev(struct inode *inode,struct file *file)
{
    if((buffk=kmalloc(1000,GFP_KERNEL))==0)
    {
        printk(KERN_INFO "The Memory cannot be alloacted to Kernel\n");
        return 0;
    }
    printk(KERN_INFO "Device Opened Successfully...\n");
    return 0;
}
static int closedev(struct inode *inode,struct file *file)
{
    kfree(buffk);
    printk(KERN_INFO "Device Closed Successfully...\n");
    return 0;
}
static ssize_t readcharacters(struct file *flip,char __user *buff,size_t len,loff_t *off)
{
    int i;
    for(i=0;i<strlen(buffk);i++)
    {
        if(buffk[i]>='a'&&buffk[i]<='z')
            buffk[i]=buffk[i]+'A'-'a';
    }
    copy_to_user(buff,buffk,strlen(buffk));
    printk(KERN_INFO "Data Read by user successfully\n");
    return strlen(buffk)-1;
}
static ssize_t writecharacters(struct file *flip,const char *buff,size_t len,loff_t *off)
{
    copy_from_user(buffk,buff,len);
    buffk[len]='\0';
    printk(KERN_INFO "Data Written by user successfully\n");
    return len;
}
static int __init initializer(void)
{
    if(alloc_chrdev_region(&d,0,1,"capital_device")<0)
    {
        printk(KERN_INFO "Major Number not allocated\n");
        return 0;
    }
    printk(KERN_INFO "Major : %d Minor : %d",MAJOR(d),MINOR(d));
    cdev_init(&mydev,&fops);
    if((cdev_add(&mydev,d,1))<0)
    {
        printk(KERN_INFO "Device adding failed");
        unregister_chrdev_region(d,1);
        return 0;
    }
    if((device_class=class_create(THIS_MODULE,"class"))==NULL)
    {
        printk(KERN_INFO "Class Creation Failed");
        unregister_chrdev_region(d,1);
        return 0;
    }
    if((device_create(device_class,NULL,d,NULL,"capital_device"))==NULL)
    {
        printk(KERN_INFO "Device Creation Failed...\n");
        class_destroy(device_class);
        return 0;
    }
    printk(KERN_INFO "Device Inserted Successfully...\n");
    return 0;
}
static void __exit exiter(void)
{
    device_destroy(device_class,d);
    class_destroy(device_class);
    cdev_del(&mydev);
    unregister_chrdev_region(d,1);
    printk(KERN_INFO "Device Removed Successfully...\n");
}
module_init(initializer);
module_exit(exiter);
