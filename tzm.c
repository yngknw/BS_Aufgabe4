/*
 * tzm.c
 *
 *  Created on: 10.01.2018
 *      Author: bs
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

struct device {
	char data[100];
	struct semaphore sem;
} virtual_device;

struct cdev *mycdev;

int major_number;
int ret;
dev_t dev_num;

#define DEVICE_NAME "tzm"

int device_open(struct inode *inode, struct file *filp) {
	if(down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "tzm: could not lock device during open\n");
		return -1;
	}
	printk(KERN_INFO "tzm: device opened\n");
	return 0;
}
ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
	printk(KERN_INFO "tzm: reading from device\n");
	ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
	return ret;
}
//ssize_t (*write) (struct file *, const char *, size_t, loff_t *);
ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* curOffset) {
	printk(KERN_INFO "tzm: writing to device\n");
	ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
	return ret;
}

int device_close(struct inode *inode, struct file *filp) {
	up(&virtual_device.sem);
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
	sema_init(&virtual_device.sem, 1);
	return 0;
}

static void driver_exit(void) {
	cdev_del(mycdev);

	unregister_chrdev_region(dev_num, 1);
	printk(KERN_INFO "tzm: module unloaded\n");
}

module_init(driver_entry);
module_exit(driver_exit);
