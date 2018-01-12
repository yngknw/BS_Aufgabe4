/*
 * tzm.c
 *
 *  Created on: 10.01.2018
 *      Author: bs
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>//for params
//#include <sys/types.h> //for ssize_t
#include <linux/fs.h> //filesystem
#include <linux/cdev.h> //for chardevice
#include <linux/fcntl.h>
#include <linux/mutex.h>//mutex
#include <linux/jiffies.h>

#include <asm/uaccess.h>//copy_to_user copy_from_user

MODULE_LICENSE("GPL");
#define DEVICE_NAME "tzm"

int ret_val_time = -1;
int ret_val_number = -1;
module_param(ret_val_time, int, 0644);
module_param(ret_val_number, int, 0644);

static DEFINE_MUTEX(mutex);
struct cdev *mycdev;
int oldTime;
int major_number;
int ret;
dev_t dev_num;

struct device {
	char data[100];
	int readingFrom;
	int writingTo;
} virtual_device;


int device_open(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "tzm: device opened\n");
	//jiffies_to_msecs(get_jiffies_64());
	return 0;
}
ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
	printk(KERN_INFO "tzm: reading from device\n");
	mutex_lock(&mutex);
	ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
	if(ret != 0) {
		printk(KERN_ALERT "copy_to_user failed");
		return -EFAULT;
	}
	mutex_unlock(&mutex);
	return ret;
}
ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* curOffset) {
	printk(KERN_INFO "tzm: writing to device\n");
	mutex_lock(&mutex);
	ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
	if(ret != 0) {
		printk(KERN_ALERT "copy_from_user failed");
		return -EFAULT;
	}
	mutex_unlock(&mutex);
	return ret;
}

int device_close(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "tzm: device closed\n");
	return 0;
}


struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = device_open,
		.release = device_close,
		.write = device_write,
		.read = device_read
};



static int driver_entry(void) {
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if(ret < 0) {
		printk(KERN_ALERT "tzm: failed to allocate major number\n");
		return ret;
	}
	major_number = MAJOR(dev_num);
	printk(KERN_INFO "tzm: device name : %s, major number : %d\n", DEVICE_NAME, major_number);

	mycdev = cdev_alloc();
	mycdev->ops = &fops;
	mycdev->owner = THIS_MODULE;

	ret = cdev_add(mycdev, dev_num, 1);
	if(ret < 0) {
		printk(KERN_ALERT "tzm: couldn't add char device to kernel\n");
		return ret;
	}

	mutex_init(&mutex);
	//virtual_device->readingFrom = 0;
	//virtual_device->writingTo = 0;

	return 0;
}

static void driver_exit(void) {
	cdev_del(mycdev);
	unregister_chrdev_region(dev_num, 1);
	mutex_destroy(&mutex);
	printk(KERN_INFO "tzm: module unloaded\n");
}

module_init(driver_entry);
module_exit(driver_exit);
