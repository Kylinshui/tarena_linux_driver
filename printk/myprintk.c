/*************************************************************************
	> File Name: printk.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月29日 星期二 11时02分59秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>

#define BDEBUG

#ifdef BDEBUG
#define BDebug(fmt,args...) printk(KERN_DEBUG "bshui:" fmt,##args)
#else
#define BDebug(fmt,args...)
#endif

static int __init myprintk_init(void){
	
	BDebug("myprintk init-----\n");
	return 0;
}

static void __exit myprintk_exit(void){

	BDebug("myprintk exit-----\n");
}

module_exit(myprintk_exit);
module_init(myprintk_init);
MODULE_LICENSE("GPL");
