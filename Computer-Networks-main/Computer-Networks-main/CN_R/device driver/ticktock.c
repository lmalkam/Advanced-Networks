#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/string.h>
#include<linux/uaccess.h>
#include<linux/time.h>
#include<linux/fs.h>
#include<linux/slab.h>


static ssize_t readtime(struct file *,char *,size_t ,loff_t *);
static ssize_t writetime(struct file *,const char *,size_t ,loff_t *);
static int open_dev(struct inode *,struct file *);
static int closedev(struct inode *,struct file *);
int major;

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char *b;

int atoi(const char * str)
{
    int res = 0,i=0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}

static struct file_operations fops={
    .open=open_dev,
    .release=closedev,
    .read=readtime,
    .write=writetime, 
};

static ssize_t readtime(struct file *filep,char *buff,size_t len,loff_t *f_ops)
{
   char *b=kmalloc(40*sizeof(char),GFP_KERNEL);
   struct timeval t1;
   do_gettimeofday(&t1);
   sprintf(b,"%ld",t1.tv_sec);
   copy_to_user(buff,b,strlen(b));
   printk("Reading\n");  
   return len;
}




ssize_t writetime(struct file *filp,const char *buff,size_t len, loff_t *off)
{
  char *b=kmalloc(30*sizeof(char),GFP_KERNEL);
  copy_from_user(b,buff,len);
  long int i=atoi(b);
  struct timespec ab;
  ab.tv_sec=i;
  ab.tv_nsec=0;
 // printk("%s",buff);
  do_settimeofday(&ab);
  return len;
}


static int open_dev(struct inode *a,struct file *filep)
{
  printk("Ticktock device has been opened..\n");
  return 0;
}

static int closedev(struct inode *a,struct file *filep)
{
  printk("Ticktock device closed just now..\n");
  return 0;
}

static int __init tick_tock_start(void)
{
   if((major=(register_chrdev(0,"mydev1",&fops)))<0)
   {
     goto fail;
   }
   printk("Ticktock device has been loaded...%d\n",major);
   return 0;
   fail:
   return -1;
}

static void __exit tick_tock_end(void)
{
   unregister_chrdev(major,"mydev1");
   printk("Ticktock device unloaded just now..\n");
}

module_init(tick_tock_start);
module_exit(tick_tock_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("3BTCSENITW");
MODULE_DESCRIPTION("TICK TOCK TIME");
