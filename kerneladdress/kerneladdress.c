/*************************************************************************
	> File Name: kernelspacekmalloc.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月25日 星期五 15时30分28秒
	>　在内核模块中使用kmalloc,__get_free_pages,vmalloc动态申请内存空间
	>　比较三种方式申请空间地址的区别
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

static unsigned char *kernelkmalloc = NULL;
static unsigned char *kernelpagemem = NULL;
static unsigned char *kernelvmalloc = NULL;
static int order = 1;

static int __init kmalloc_init(void){
	int ret = -ENOMEM;
	kernelkmalloc = (unsigned char *)kmalloc(100, GFP_KERNEL);
	if(IS_ERR(kernelkmalloc)){
		printk("Failure to allocate memory from kernel\n");
		return ret;
	}

	printk("Success to get memory using kmalloc from kernel Address:0x%lx\n",(unsigned long)kernelkmalloc);

	kernelpagemem = (unsigned char *)__get_free_pages(GFP_KERNEL,order);

	if(IS_ERR(kernelpagemem)){
		printk("Failure to allocate on page memory from kernel\n");
		return ret;
	}

	printk("Success to get free pages from kernel Address:0x%lx\n",
				(unsigned long)kernelpagemem);

	kernelvmalloc = (unsigned char *)vmalloc(1024*1024);
	if(IS_ERR(kernelvmalloc)){
		printk("Cann't get memory using vmalloc from kernel\n");
		return ret;
	}

	printk("Success to get vmalloc from kernel Address:0x%ld\n",
				(unsigned long)kernelvmalloc);

	return 0;
}

static void __exit kmalloc_exit(void){
	vfree(kernelvmalloc);
	kernelvmalloc = NULL;

	free_pages((unsigned long)kernelpagemem,order);
	kernelpagemem = NULL;

	kfree(kernelkmalloc);
	kernelkmalloc = NULL;
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);

MODULE_LICENSE("GPL");
