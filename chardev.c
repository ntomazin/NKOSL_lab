#include<linux/init.h>

#include<linux/module.h>

#include<linux/fs.h>

#include<linux/string.h>

#include<asm/uaccess.h>

MODULE_LICENSE(“GPL”);

 

#define DEVICE_NAME "chardev"
#define BUF_LEN 80

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;
static char ker_buf[100];   //driver local buffer

 

static int dev_open(struct inode *inod, struct file *fil);

static ssize_t dev_read(struct file *filep,char *buf,size_t len,loff_t *off);

static ssize_t dev_write(struct file *flip,const char *buff,size_t len,loff_t *off);

static int dev_release(struct inode *inod,struct file *fil);




//structure containing device operation

static struct file_operations fops=

{

.read=dev_read, //pointer to device read funtion

.write=dev_write, //pointer to device write function

.open=dev_open,   //pointer to device open function

.release=dev_release, //pointer to device realese function

};

 

 

static int hello_init(void)   //init function to be called at the time of insmod

{

  Major = register_chrdev(0, DEVICE_NAME, &fops);

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

  return 0;

}

static void hello_exit(void) //exit function to be called at the time of rmmod

{

unregister_chrdev(Major, DEVICE_NAME);

printk(KERN_ALERT “exit”);

}

 

static int dev_open(struct inode *inod, struct file *fil)
{

    if (Device_Open)
        return -EBUSY;

    Device_open++;
    printk("Good afternoon, gentleman. I am HAL 9000, computer.\n");
    return 0;



}

 

static ssize_t dev_read(struct file *filep,char *buf,size_t len,loff_t *off)

{
    static int counter = 0;
    //brojac koliko je puta dave zarazio citanje PID-a i vratiti pid pozivatelja
    printk("Affirmative Dave, I read you.\n");
    printk("Dave, you PID is %d.\n", (int) task_pid_nr(current));
    printk("I told you this information %d times.\n", counter++);
    return 0;

}

 

static ssize_t dev_write(struct file *flip,const char *buf,size_t len,loff_t *off)

{
/*
copy_from_user(ker_buf,buf,len);

ker_buf[len]=0;

return len;
*/

    

    printk("I'm sorry Dave, I am afraid I can't do that.\n");
    return -EINVAL;
}

 

static int dev_release(struct inode *inod,struct file *fil)

{
    Dvice_Open--;
    printk("I am afraid. I am afraid Dave.\n");
    return 0;

}

 

module_init(hello_init);

module_exit(hello_exit);
