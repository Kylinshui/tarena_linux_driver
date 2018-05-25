/*************************************************************************
	> File Name: kernelspacekmalloc.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月25日 星期五 15时30分28秒
	>　在内核模块中使用kmalloc,get_zeroed_page动态申请内存空间
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>

static char *buf1 = NULL;//kmalloc
static char *buf2 = NULL;//get_zeroed_page 

static int __init kmalloc_init(void){
	buf1 = (char *)kmalloc(100, GFP_KERNEL);
	if(IS_ERR(buf1)){
		printk("Failure to allocate memory from kernel\n");
		kfree(buf1);
		buf1 = NULL;
	}
	memset(buf1,0,100);
	strcpy(buf1, "Success to allocate memory from kernel:100 bytes\n");
	printk("buf1:%s\n",buf1);

	buf2 = (char *)get_zeroed_page(GFP_KERNEL);
	if(IS_ERR(buf2)){
		printk("Failure to allocate on page memory from kernel\n");
		return -ENOMEM;
	}

	strcpy(buf2, "Success to allocate one page memory form kernel\n");
	printk("Buffer buf2:%s\n", buf2);

	return 0;
}

static void __exit kmalloc_exit(void){
	free_page((unsigned long)buf2);
	buf2 = NULL;
	kfree(buf1);
	buf1 = NULL;
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);

MODULE_LICENSE("GPL");
