/*************************************************************************
	> File Name: chardevicedriver.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 14时48分26秒
	> mknod /dev/chardevicedriver c 240 0
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define CHARDEVICEDRIVER_MAJOR 240
#define CHARDEVICEDRIVER_MINOR 0
#define CHARDEVICEDRIVER_COUNT 1
#define CHARDEVICEDRIVER_NAME "chardevicedriver"

dev_t dev = 0;

static int __init chardevicedriver_init(void){
	int ret = -EFAULT;
	//register device number
	dev = MKDEV(CHARDEVICEDRIVER_MAJOR,CHARDEVICEDRIVER_MINOR);

	ret = register_chrdev_region(dev, CHARDEVICEDRIVER_COUNT,CHARDEVICEDRIVER_NAME);

	if(ret < 0){
		printk("CharDeviceDriver cann't register char device\n");
		return ret;
	}

	printk("CharDeviceDriver:Success to register char device\n");

	return ret;
}

static void __exit chardevicedriver_exit(void){
	unregister_chrdev_region(dev,CHARDEVICEDRIVER_COUNT);

}

module_init(chardevicedriver_init);
module_exit(chardevicedriver_exit);
MODULE_LICENSE("GPL");
