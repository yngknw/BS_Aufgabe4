/*
 * hello.c
 *
 *  Created on: 10.01.2018
 *      Author: bs
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");


int param = 1;
int param2 = 2;
module_param(param, int, 0644);
module_param(param2, int, 0644);

void display_param(void) {

//	printk(KERN_ALERT "param = %d", param);
}

static int hello_init(void) {
	printk(KERN_DEBUG "Hallo Elias");
	printk(KERN_INFO "Hallo Hallo Hallo Hallo Hallo Hallo");
	printk(KERN_DEBUG "param = %d, param2 = %d", param, param2);
//	display_param();
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Tschüss Leute");
}

module_init(hello_init);
module_exit(hello_exit);
