/*
 * main.c
 *
 * test char driver written while following ldd3
 *  Created on: Oct 3, 2012
 *      Author: sinan
 * Copyright (C) 2012 Sinan Akpolat
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/types.h> //for dev_t, dev_t means device number
#include <linux/fs.h> //for register and unregister dev region stuff
#include <linux/cdev.h>

#include <asm/uaccess.h> //copy_to_user, copy_from_user userspace access functions


MODULE_LICENSE("GPL");

static char embedstr[462] = "Hello, world! Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n";

static dev_t mydevnum;
static struct cdev mydev;


static int chtest_open(struct inode* inodeptr, struct file *fptr) {
	printk(KERN_ALERT "device file opened for the first time.");
	printk(KERN_ALERT "but there's nothing I'm gonna do about it.");
	//we don't have to know which device is opened, we don't need to do anything.
	return 0;
}

static int chtest_release(struct inode* inodeptr, struct file *fptr) {
	//similarly close the file. We are sure there is no one reading or writing to it.
	//So what? again we don't have to do anything.
	printk(KERN_ALERT "device file is closed for good,");
	printk(KERN_ALERT "no one is using it right now.");
	return 0;
}

static ssize_t chtest_read(struct file *fptr, char __user *uspptr, size_t count, loff_t* fpos) {
	//it doesn't matter where you wanna read, I'll always give you the same string!!!
	//I want to copy a string to userspace, so I use copy_to_user(*to pointer, *from pointer, ulong count)
	//it's quite similar to memcpy
	if(count>462) count = 462;
	if(copy_to_user(uspptr, embedstr, count)) {
		printk(KERN_ALERT "memcpy to userspace failed :/");
		return -EFAULT;
	}
	else return count;
}

static ssize_t chtest_write(struct file *fptr, const char __user *uspptr, size_t count, loff_t* fpos) {
	/*
	 * We are not interested in writing right now. But let's make it seem like the device
	 * writes the desired number of bytes everytime.
	 */
	return count; //let's hope it's not big enough to return negative values :)
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = chtest_open,
	.release = chtest_release,
	.read = chtest_read,
	.write = chtest_write
	//I'll also add a llseek and ioctl handler function sometime
}; //create fops struct with init values just like initializing an array

static int chtest_init(void) {
	int error = 0;
	//we should take major and minor numbers dynamically but I'll use static for sake of simplicity
	dev_t mydevnum = MKDEV(100,0); //you can check for free major numbers on /proc/devices
	register_chrdev_region(mydevnum, 1, "chtest");  //register the numbers
	//we can also allocate mydev dynamically
	cdev_init(&mydev, &f_ops);
	mydev.owner = THIS_MODULE;
	error = cdev_add(&mydev, mydevnum, 1);
	if(error) {
		printk(KERN_ALERT "unable to add device errno: %d\n", error);
		unregister_chrdev_region(mydevnum, 1);
		return error;
	}
	printk(KERN_ALERT "chtest module loaded!\n");
	return 0;
}

static void chtest_exit(void) {

	cdev_del(&mydev); //take the device out of the system
	unregister_chrdev_region(mydevnum, 1); //unregister our dev numbers
	printk(KERN_ALERT "chtest module unloaded...\n");
}

module_init(chtest_init);
module_exit(chtest_exit);
