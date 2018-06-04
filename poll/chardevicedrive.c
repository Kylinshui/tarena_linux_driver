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
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#include <linux/sched.h>

struct class *char_class;
#define DEVICE_NAME "chardev"
#define CHARMAJOR 300
#define CHARMINOR 0
//对幻数的编号千万不能重复定义，如ioctl-number.txt
//_IO(type,nr)定义无参数的命令
#define MISC_IOC_MAGIC 'M'
#define LED_ON  _IO(MISC_IOC_MAGIC, 0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC, 0x02)

wait_queue_head_t wqh;
u8 led=0;
static int chardevicedriver_open(struct inode *inode, struct file *filp){
	printk("%s---------Entry\n",__func__);

	return 0;
}

static ssize_t chardevicedriver_read(struct file *filp, char __user *buf,
			size_t count, loff_t *f_pos){
	led = 0;
	return count;
}

static ssize_t chardevicedriver_write(struct file *filp, const char __user *buf,size_t count, loff_t *f_pos){
	led = 1;
	
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

static unsigned int chardevicedriver_poll(struct file *filp,struct poll_table_struct *wait){
	unsigned int mask = 0;
	//call poll_wait()
	poll_wait(filp, &wqh,wait);//把进程添加到轮询表
	//set return status
	//如果条件满足读
	//POLLIN 设备可无阻塞读 POLLRDNORM 数据可读
	if(led!=0){
		mask |= POLLIN | POLLRDNORM;
		printk("Device is ready for reading Mask:=0x%4x\n", mask);
	}else{
		printk("Device is not ready for reading Mask:=0x%4x\n",mask);
	}

	return mask;
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
	.poll  = chardevicedriver_poll,
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
	init_waitqueue_head(&wqh);
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
