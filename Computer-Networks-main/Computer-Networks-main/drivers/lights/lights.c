/*
 * chardev.c: Creates a read-only char device that says how many times
 * you've read from the dev file
 */
#include <linux/kernel.h>

#include <linux/module.h>

#include <linux/fs.h>

#include <asm/io.h>

#include <linux/delay.h>

#include <asm/uaccess.h> /* for put_user */

MODULE_LICENSE("GPL");

/*
 * Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode * , struct file * );
static int device_release(struct inode * , struct file * );
static ssize_t device_read(struct file * , char * , size_t, loff_t * );
static ssize_t device_write(struct file * ,
  const char * , size_t, loff_t * );
#define SUCCESS 0
#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80 /* Max length of the message from the device */
/*
 * Global variables are declared as static, so are global within the file.
 */
static int Major; /* Major number assigned to our device driver */
static int Device_Open = 0;
/* Is device open?
 * Used to prevent multiple access to device */
static char msg[BUF_LEN]; /* The msg the device will give when asked */
static char * msg_Ptr;
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};
/*
 * This function is called when the module is loaded
 */
int init_module(void) {
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
/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void) {
  /*
   * Unregister the device
   */
  unregister_chrdev(Major, DEVICE_NAME);
}
/*
 * Methods
 */
/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode * inode, struct file * file) {
  static int counter = 0;
  if (Device_Open)
    return -EBUSY;
  Device_Open++;
  sprintf(msg, "I already told you %d times Hello world!\n", counter++);
  msg_Ptr = msg;
  
  
  int retries = 5;
  int timeout = 5000;
  
  int state = 1<<2;
  /*outb(0xed,0x60);
  udelay(timeout);
  while(retries != 0 && inb(0x60) != 0xfa){
  	retries--;
  	udelay(timeout);
  }
  if(retries){
  	outb(state,0x60);
  	printk("caps on ig\n");
  	udelay(timeout); 
 	printk("Init delay done\n");
  	retries = 15;
  	outb(0xed,0x60);
  	udelay(timeout);
  	while(retries != 0 && inb(0x60) != 0xfa){
  		retries--;
  		printk("At try : %d\n",retries);
  		udelay(timeout);
  	}
  	printk("Retries left : %d\n",retries);
  	if(retries){
  		state = 1<<0 | 1<<1;
  		outb(state,0x60);
  	}
  }*/
  
  for(int i = 0; i < 10; ++i){
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
	outb(1<<1,0x60);
	mdelay(timeout);
  }
  
  try_module_get(THIS_MODULE);
  return SUCCESS;
}
/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode * inode, struct file * file) {
  Device_Open--; /* We're now ready for our next caller */
  /*
   * Decrement the usage count, or else once you opened the file, you'll
   * never get get rid of the module.
   */
  module_put(THIS_MODULE);
  return 0;
}
/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file * filp, /* see include/linux/fs.h */
  char * buffer, /* buffer to fill with data */
  size_t length, /* length of the buffer */
  loff_t * offset) {
  /*
   * Number of bytes actually written to the buffer
   */
  int bytes_read = 0;
  /*
   * If we're at the end of the message,
   * return 0 signifying end of file
   */
  if ( * msg_Ptr == 0)
    return 0;
  /*
   * Actually put the data into the buffer
   */
  while (length && * msg_Ptr) {
    /*
     * The buffer is in the user data segment, not the kernel
     * segment so "*" assignment won't work. We have to use
     * put_user which copies data from the kernel data segment to
     * the user data segment.
     */
    put_user( * (msg_Ptr++), buffer++);
    length--;
    bytes_read++;
  }
  /*
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}
/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t
device_write(struct file * filp,
  const char * buff, size_t len, loff_t * off) {
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}
