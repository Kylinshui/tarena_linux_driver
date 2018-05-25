/*************************************************************************
	> File Name: hello_temp.c
	> Author: shuixianbing
	> Mail: shui6666@126.com 
	> Created Time: 2018年03月12日 星期一 13时44分17秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");


static int __init hello_init(void){

	printk("hello init\n");
	return 0;
}

static void __exit hello_exit(void){

	printk("hello_exit\n");
}
module_init(hello_init);
module_exit(hello_exit);

