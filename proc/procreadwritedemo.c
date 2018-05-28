/*************************************************************************
	> File Name: procreadwritedemo.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 10时28分18秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

static struct proc_dir_entry *prwd_dir = NULL;
static struct proc_dir_entry *prwd_rdonly = NULL;
static struct proc_dir_entry *prwd_rdwr = NULL;

static char sbufr[100] = "One test string for reading only\n";
static char sbufrw[100] = "";

//read-only function
static int prwd_rdonly_read(char *page, char **start, off_t off, int count,
			int *eof, void *data){
	sprintf(page, "%s", sbufr);
	*eof = 1;

	return strlen(sbufr);
}

//read function for read-write
static int prwd_rdwr_read(char *page, char **start, off_t off, int count, int *eof, void *data){
	int len;
	len = strlen(sbufrw);
	if(len <= off){
		printk("No data for reading in read-write buffer\n");
		return -EINVAL;
	}

	if((len-off)<count){
		count = len - off;
	}

	memcpy((page+off),(sbufrw+off),count);
	if(len<=(off+count)){
		*eof = 1;
	}
	printk("-----%s-----\n",__func__);
	return (off+count);
}

//write function for read-write
static int prwd_rdwr_write(struct file *file, const char __user *buffer,
			unsigned long count, void *data){
	unsigned long cnt = count;
	if(cnt >= sizeof(sbufrw)){
		cnt = sizeof(sbufrw)-1;
	}

	if(copy_from_user(sbufrw, buffer, cnt)){
		printk("Failure to copy data from user space\n");
		return -EFAULT;
	}

	sbufrw[cnt] = '\0';
	printk("-----%s-----\n",__func__);
	return cnt;
}

static int __init procreadwrite_init(void){
	//create directory procreadwrite in /proc 
	prwd_dir = proc_mkdir("procreadwritedemo", NULL);
	if(NULL == prwd_dir){
		printk("cann't create directiory /proc/procreadwritedemo\n");
		return -EFAULT;
	}

	printk("Success to create directory /proc/procreadwritedemo\n");
	//create read-only file 
	//prwd_rdonly_read 文件的读函数指针
	prwd_rdonly = create_proc_read_entry("rdonly",0400,prwd_dir,prwd_rdonly_read,NULL);

	if(NULL==prwd_rdonly){
		printk("Cann't create file:rdonly in /proc/procreadwritedemo\n");
		remove_proc_entry("procreadwritedemo",NULL);
	}
	
	printk("Success to create file:rdonly in /proc/procreadwritedemo\n");

	//create read-write file in /proc/procreadwritedemo
	prwd_rdwr = create_proc_entry("rdwr",0666,prwd_dir);
	if(NULL==prwd_rdwr){
		printk("Cann't create file:rdwr in /proc/procreadwritedemo\n");
		remove_proc_entry("rdonly",prwd_dir);
	}
	prwd_rdwr->read_proc  = prwd_rdwr_read;
	prwd_rdwr->write_proc = prwd_rdwr_write;

	printk("Success to create file:rdwr in /proc/procreadwritedemo\n");

	return 0;
}

static void __exit procreadwrite_exit(void){
	//remove files and directory created in /proc 
	remove_proc_entry("rdwr", prwd_dir);
	remove_proc_entry("rdonly", prwd_dir);
	remove_proc_entry("procreadwritedemo", NULL);
}

module_init(procreadwrite_init);
module_exit(procreadwrite_exit);
MODULE_LICENSE("GPL");
