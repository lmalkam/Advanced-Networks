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
#include <asm/io.h>
#include <linux/delay.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRATHAM BANDE");
MODULE_DESCRIPTION("Calculating Basic Equation and making Lights Blink !!");

static int __init initializer(void);
static void __exit exiter(void);
int Major;
#define SUCCESS 0
#define DEVICE_NAME "calculator"
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
    int st1[100];
    int top1 = -1;
    char st2[100];
    char top2 = -1;
    
    int i = 0;    
    int lenofbuff = strlen(buffk)-1;
    int valid = 1;
    printk(KERN_INFO "lenofbuff : %d\n", lenofbuff);
    while(i < lenofbuff){
    	if(buffk[i] == ' '){
    		++i;
    		continue;
    	}
    	if(buffk[i] <= '9' && buffk[i] >= '0'){
    		int number = 0;
    		while((i < len) && (buffk[i] <= '9' && buffk[i] >= '0')){
    			number *= 10;
    			number += buffk[i]-'0';
    			++i;
    		}
    		top1++;
    		st1[top1] = number;
    	}
    	else if(buffk[i] == '*' || buffk[i] == '/' || buffk[i] == '('){
    		top2++;
    		st2[top2] = buffk[i++];
    	}
    	else if(buffk[i] == '+' || buffk[i] == '-'){
    		while((top2 >= 0) && (st2[top2] != '(')){
    			if(top1 < 0){valid = 0; break;}
    			int num1 = st1[top1--];
    			if(top1 < 0){valid = 0; break;}
    			int num2 = st1[top1];
    			if(st2[top2] == '*')st1[top1] = num1*num2;
    			if(st2[top2] == '+')st1[top1] = num1+num2;
    			if(st2[top2] == '-')st1[top1] = num2-num1;
    			if(st2[top2] == '/'){
    			if(num1 == 0){
    				valid = 0;
    				break;
    			}
    			st1[top1] = num2/num1;
    			}
    			top2--;
    		}	
    		top2++;
    		st2[top2] = buffk[i++];
    	}else if(buffk[i] == ')'){
    		while(top2 >= 0 && st2[top2] != '('){
    			if(top1 < 0){valid = 0; break;}
    			int num1 = st1[top1--];
    			if(top1 < 0){valid = 0; break;}
    			int num2 = st1[top1];
    			if(st2[top2] == '*')st1[top1] = num1*num2;
    			if(st2[top2] == '+')st1[top1] = num1+num2;
    			if(st2[top2] == '-')st1[top1] = num2-num1;
    			if(st2[top2] == '/'){
    			if(num1 == 0){
    				valid = 0;
    				break;
    			}
    			st1[top1] = num2/num1;
    			}
    			top2--;
    		}
    		if(top2 < 0){
    			valid = 0;
    			break;
    		}
    		--top2;
    		++i;
    	}else{
    		valid = 0;
    		break;
    	}
    }
    
    if(valid){
    	while(top2 >= 0){
    		if(top1 < 0){valid = 0; break;}
    		int num1 = st1[top1--];
    		if(top1 < 0){valid = 0; break;}
    		int num2 = st1[top1];
    		if(st2[top2] == '*')st1[top1] = num1*num2;
    		if(st2[top2] == '+')st1[top1] = num1+num2;
    		if(st2[top2] == '-')st1[top1] = num2-num1;
    		if(st2[top2] == '/'){
    			if(num1 == 0){
    				valid = 0;
    				break;
    			}
    			st1[top1] = num2/num1;
    		}
    		top2--;
    	}
    }
    
    if(top1 != 0)valid = 0;
    int toret;
    if(valid){
    	char res[100];
    	int num = st1[top1], sv = st1[top1];
    	int idx = 0;
 	
 	if(num < 0)num *= -1;
 	if(!num)
 		res[idx++] = '0';
    	while(num){
    		res[idx++] = (num%10)+'0';
    		num /= 10;
    	}
    	if(sv < 0)
    		res[idx++] = '-';
    	res[idx] = '\0';
    	for(int j = 0; j < idx/2; ++j){
    		char t = res[j];
    		res[j] = res[idx-j-1];
    		res[idx-j-1] = t;
    	}
    	copy_to_user(buff,res,strlen(res)+1);
    	toret = strlen(res);
    	
    	int timeout = 100;
    	if(sv < 0){
    		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(1<<2 | 1<<1 | 1,0x60);
		mdelay(timeout);
		
		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(0,0x60);
		mdelay(timeout);
		
		sv *= -1;
    	}
    	for(int i = 0; i < sv; ++i){
    		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(1<<1,0x60);
		mdelay(timeout);
		
		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(1<<2,0x60);
		mdelay(timeout);
		
		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(1<<0,0x60);
		mdelay(timeout);
    	}
    	
    }else{
    	char res[] = "Invalid Infix Expression";
    	toret = strlen(res);
    	copy_to_user(buff,res,strlen(res));
    	int timeout = 250;
    	for(int i = 0; i < 3; ++i){
    		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(1<<2 | 1<<1 | 1,0x60);
		mdelay(timeout);
		
		outb(0xed,0x60);
		mdelay(timeout);
		while(inb(0x60) != 0xfa)
			mdelay(timeout);
		outb(0,0x60);
		mdelay(timeout);
    	}
    }	
    printk(KERN_INFO "Data Read by user successfully\n");
    return toret;
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
  Major = register_chrdev(0, DEVICE_NAME, & fops);
  if (Major < 0) {
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    return Major;
  }
  printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
  printk(KERN_INFO "the driver, create a dev file with\n");
  printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
  printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
  printk(KERN_INFO "the device file.\n");
  printk(KERN_INFO "Remove the device file and module when done.\n");
  return SUCCESS;
}

static void __exit exiter(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "Device Removed Successfully...\n");
}
module_init(initializer);
module_exit(exiter);

