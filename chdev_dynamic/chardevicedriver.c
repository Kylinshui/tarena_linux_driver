/*************************************************************************
	> File Name: chardevicedriver.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 14时48分26秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

struct class *char_class;
#define DEVICE_NAME "chardev"
#define CHARMAJOR 300
#define CHARMINOR 0

static int chardevicedriver_open(struct inode *inode, struct file *filp){
	printk("%s---------Entry\n",__func__);

	return 0;
}

static ssize_t chardevicedriver_read(struct file *filp, char __user *buf,
			size_t count, loff_t *f_pos){
	
	printk("%s---------Entry\n",__func__);
	return 0;
}

static ssize_t chardevicedriver_write(struct file *filp, const char __user *buf,
			size_t count, loff_t *f_pos){
	
	printk("%s---------Entry\n",__func__);
	return 0;
}

static int chardevicedriver_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg){
	
	printk("%s---------Entry\n",__func__);

	return 0;
}

static int chardevicedriver_release(struct inode *inode, struct file *filp){
	
	printk("%s---------Entry\n",__func__);

	return 0;
}
static struct file_operations chardevicedriver_fops = {
	.owner = THIS_MODULE,
	.open  = chardevicedriver_open,
	.read  = chardevicedriver_read,
	.write = chardevicedriver_write,
	.unlocked_ioctl = chardevicedriver_ioctl,
	.release = chardevicedriver_release,
};

static int __init chardevicedriver_init(void){
	int ret;

	ret = register_chrdev(CHARMAJOR,DEVICE_NAME,&chardevicedriver_fops);
	if(ret < 0){
		printk("unable to register chardevicedriver \n");
		return ret;
	}

	char_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(char_class)){
		printk("char class create fail\n");
	}

	device_create(char_class,NULL,MKDEV(CHARMAJOR,CHARMINOR),NULL,DEVICE_NAME);

	return ret;
}

static void __exit chardevicedriver_exit(void){
	device_destroy(char_class,MKDEV(CHARMAJOR,CHARMINOR));
	class_destroy(char_class);
	unregister_chrdev(CHARMAJOR,DEVICE_NAME);

}

module_init(chardevicedriver_init);
module_exit(chardevicedriver_exit);
MODULE_LICENSE("GPL");
