/*************************************************************************
	> File Name: virtualblockdevice.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年06月04日 星期一 14时25分15秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#define VIRTUALBLOCKDEVICE_MAJOR 0
#define VIRTUALBLOCKDEVICE_MINOR 0
#define VIRTUALBLOCKDEVICE_NAME "virtualblkdev0"

static int __init virtualblockdevice_init(void){
	int ret = 0;
	//块设备驱动注册
	ret = register_blkdev(VIRTUALBLOCKDEVICE_MAJOR, VIRTUALBLOCKDEVICE_NAME);
	if(ret < 0){
		printk("VIrtualBLockDevice:Failure to register block device\n");
		return ret;
	}

	printk("VirtualBlockDevice:Success to register block device\n");
	return ret;
}

static void __exit virtualblockdevice_exit(void){
	unregister_blkdev(VIRTUALBLOCKDEVICE_MAJOR,VIRTUALBLOCKDEVICE_NAME);
}

module_init(virtualblockdevice_init);
module_exit(virtualblockdevice_exit);
MODULE_LICENSE("GPL");
