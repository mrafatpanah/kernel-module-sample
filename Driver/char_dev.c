#include "baselib.h"

struct fake_device{
  char data[100];
  struct semaphore sem;
} virtual_device;

struct cdev *mycdev;
int major_number;
int ret;

dev_t dev_num;

#define DEVICE_NAME     "char_dev"

//this function use for open device driver file
int device_open(struct inode *inode, struct file *filep){
  if(down_interruptible(&virtual_device.sem) != 0){
    printk(KERN_ALERT "char_dev: could not lock device during open");
    return -1;
  }
  printk(KERN_INFO "char_dev: opened device");
  return 0;
}


//this function use for reading data received from user app (device)
ssize_t device_read(struct file* filep, char* bufStoreData, size_t bufCount, loff_t* curOffset){
  printk(KERN_INFO "char_dev: Reading from device");
  ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
  return ret;
}

//this function use for writing data to device from user app
ssize_t device_write(struct file *filep, const char *bufSourceData, size_t bufCount, loff_t *curOffset) {
  printk(KERN_INFO "char_dev: Writing to device");
  ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
  return ret;
}

//this function use for close device driver file
int device_close(struct inode *inode, struct file *filep){
  up(&virtual_device.sem);
  printk(KERN_INFO "char_dev: closed device");
  return 0;
}

//over ride file operations
struct file_operations fops = {
  .owner   = THIS_MODULE,
  .open    = device_open,
  .release = device_close,
  .write   = device_write,
  .read    = device_read
};


static int driver_entry(void){
  ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
  if(ret < 0){
    printk(KERN_ALERT "char_dev: failed to allocate a major number!");
    return ret;
  }
  major_number = MAJOR(dev_num);
  printk(KERN_INFO "char_dev: major number is %d", major_number);
  printk(KERN_INFO "\t use \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);

  mycdev = cdev_alloc();
  mycdev->ops = &fops;
  mycdev->owner = THIS_MODULE;

  ret = cdev_add(mycdev, dev_num, 1);
  if(ret < 0){
    printk(KERN_ALERT "char_dev: unabele to add cdev to kernel!");
    return ret;
  }

  sema_init(&virtual_device.sem, 1);

  return 0;
}



static void driver_exit(void){
      cdev_del(mycdev);

      unregister_chrdev_region(dev_num, 1);
      printk(KERN_ALERT "char_dev: unloaded module!");
}

module_init(driver_entry);
module_exit(driver_exit);
