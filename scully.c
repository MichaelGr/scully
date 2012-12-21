/*
 * scully.c
 * Copyright (C) 2012 Sinan Akpolat
 *
 * test char driver written while following LDD3
 * This file is distributed under GNU GPLv3, see LICENSE file.
 * If you haven't received a file named LICENSE see <http://www.gnu.org/licences>
 *
 * scully driver is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 *
 * This is a code written solely for training purposes,
 * under any circumstances it should not be run on a production system.
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


static int scully_open(struct inode* inodeptr, struct file *fptr) {
	printk(KERN_ALERT "device file opened for the first time.");
	printk(KERN_ALERT "but there's nothing I'm gonna do about it.");
	//we don't have to know which device is opened, we don't need to do anything.
	return 0;
}

static int scully_release(struct inode* inodeptr, struct file *fptr) {
	//similarly close the file. We are sure there is no one reading or writing to it.
	//So what? again we don't have to do anything.
	printk(KERN_ALERT "device file is closed for good,");
	printk(KERN_ALERT "no one is using it right now.");
	return 0;
}

static ssize_t scully_read(struct file *fptr, char __user *uspptr, size_t count, loff_t* fpos) {
	//it doesn't matter what position you want to read, it will always give you the same string!!!
	//I want to copy a string to userspace, so I use copy_to_user(*to pointer, *from pointer, ulong count)
	//it's quite similar to memcpy
	if(count>462) count = 462;
	if(copy_to_user(uspptr, embedstr, count)) {
		printk(KERN_ALERT "memcpy to userspace failed :/");
		return -EFAULT;
	}
	else return count;
}

static ssize_t scully_write(struct file *fptr, const char __user *uspptr, size_t count, loff_t* fpos) {
	/*
	 * We are not interested in writing right now. But let's make it seem like the device
	 * writes the desired number of bytes everytime.
	 */
	return count; //let's hope it's not big enough to return negative values :)
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = scully_open,
	.release = scully_release,
	.read = scully_read,
	.write = scully_write
	//We don't have llseek and ioctl handler functions
}; //create fops struct with initial values, just like initializing an array

static int scully_init(void) {
	int error = 0;
	//we should take major and minor numbers dynamically but I'll use static for sake of simplicity
	dev_t mydevnum = MKDEV(100,0); //you can check for free major numbers on /proc/devices if (100,0) is  taken
	error = register_chrdev_region(mydevnum, 1, "scully");  //register the numbers
	if(error) {
		printk(KERN_ALERT "Uanble to register device number (100, 0)");
		return error;
	}
	cdev_init(&mydev, &f_ops);
	mydev.owner = THIS_MODULE;
	error = cdev_add(&mydev, mydevnum, 1);
	if(error) {
		printk(KERN_ALERT "unable to add device errno: %d\n", error);
		unregister_chrdev_region(mydevnum, 1);
		return error;
	}
	printk(KERN_ALERT "scully module loaded!\n");
	return 0;
}

static void scully_exit(void) {

	cdev_del(&mydev); //take the device out of the system
	unregister_chrdev_region(mydevnum, 1); //unregister our dev numbers
	printk(KERN_ALERT "scully module unloaded...\n");
}

module_init(scully_init);
module_exit(scully_exit);
