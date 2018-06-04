/*************************************************************************
	> File Name: chardevicedriver.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 14时48分26秒
	> cat /sys/class/chardev/class_name
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/device.h>
#include <linux/fs.h>

struct class *char_class;
static struct device *devicemodelclass_device = NULL;
#define DEVICE_NAME "chardev"
#define CHARMAJOR 300
#define CHARMINOR 0
//对幻数的编号千万不能重复定义，如ioctl-number.txt
//_IO(type,nr)定义无参数的命令
#define MISC_IOC_MAGIC 'M'
#define LED_ON  _IO(MISC_IOC_MAGIC, 0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC, 0x02)

static int chardevicedriver_open(struct inode *inode, struct file *filp){
	printk("%s---------Entry\n",__func__);

	return 0;
}

static ssize_t chardevicedriver_read(struct file *filp, char __user *buf,
			size_t count, loff_t *f_pos){
	
	char sbuf[100] = "Test string in kernel space for writing to user space\n";
	int ret;

	ret = copy_to_user(buf, sbuf,count);
	if(ret){
		return -EFAULT;
	}
	printk("%s---------kernel space writing string to user\n",__func__);

	return count;
}

static ssize_t chardevicedriver_write(struct file *filp, const char __user *buf,
			size_t count, loff_t *f_pos){
	char sbuf[100]={0};
	//read string from user space
	int ret;

	ret = copy_from_user(sbuf,buf,count);
	if(ret)
	  return -EFAULT;

	printk("%s---------kernel space read string from user space :%s\n",__func__,sbuf);

	return count;
}

static int chardevicedriver_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg){
	if(_IOC_TYPE(cmd)!= MISC_IOC_MAGIC)
		return -EFAULT;

	switch(cmd){
		case LED_ON:
			printk("led....on\n");
			break;
		case LED_OFF:
			printk("led....off\n");
			break;
		default:
			break;
	}	

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

static ssize_t devicemodelclass_class_name_show(struct class *class, char *buf){
	printk("DeviceModelClass: %s Entry\n",__func__);

	return 0;
}

static ssize_t devicemodelclass_device_name_show(struct device *dev, struct device_attribute *attr, char *buf){
	printk("DeviceModelClass: %s Entry \n",__func__);

	return 0;
}

static CLASS_ATTR(class_name, 0400, devicemodelclass_class_name_show, NULL );
static DEVICE_ATTR(device_name, 0400, devicemodelclass_device_name_show, NULL);

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

	//create class attribute
	class_create_file(char_class, &class_attr_class_name);
	//create device attribute
	device_create_file(devicemodelclass_device,&dev_attr_device_name);

	return ret;
}

static void __exit chardevicedriver_exit(void){
	//remove device attribute 
	device_remove_file(devicemodelclass_device, &dev_attr_device_name);
	//remove class attribute 
	class_remove_file(char_class, &class_attr_class_name);
	device_destroy(char_class,MKDEV(CHARMAJOR,CHARMINOR));
	class_destroy(char_class);
	unregister_chrdev(CHARMAJOR,DEVICE_NAME);

}

module_init(chardevicedriver_init);
module_exit(chardevicedriver_exit);
MODULE_LICENSE("GPL");
