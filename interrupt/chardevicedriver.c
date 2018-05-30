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
#include <linux/interrupt.h>

struct class *char_class;
#define DEVICE_NAME "chardev"
#define CHARMAJOR 200
#define CHARMINOR 0
//对幻数的编号千万不能重复定义，如ioctl-number.txt
//_IO(type,nr)定义无参数的命令
#define MISC_IOC_MAGIC 'M'
#define LED_ON  _IO(MISC_IOC_MAGIC, 0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC, 0x02)

#define IRQ_NUM 10

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

//tasklet 处理函数
static void chardevicedriver_tasklet_func(unsigned long data){
	printk("enter chardevicedriver_tasklet_func\n");

}

//tasklet定义
DECLARE_TASKLET(chardevicedriver_tasklet,chardevicedriver_tasklet_func,NULL);

static irqreturn_t chardevicedriver_isr(int irq, void *dev_id){
	printk("enter chardevicedriver isr func irq=%d\n",irq);
	//中断底半部
	tasklet_schedule(&chardevicedriver_tasklet);
	return IRQ_HANDLED;
}

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

	ret = request_irq(IRQ_NUM,chardevicedriver_isr,0,"interrupttest",NULL);
	if(ret < 0){
		printk("Failure to request irq %d\n", IRQ_NUM);
		device_destroy(char_class,MKDEV(CHARMAJOR,CHARMINOR));
		class_destroy(char_class);
		unregister_chrdev(CHARMAJOR,DEVICE_NAME);
		return ret;
	}


	return ret;
}

static void __exit chardevicedriver_exit(void){
	device_destroy(char_class,MKDEV(CHARMAJOR,CHARMINOR));
	class_destroy(char_class);
	unregister_chrdev(CHARMAJOR,DEVICE_NAME);
	free_irq(IRQ_NUM, NULL);

}

module_init(chardevicedriver_init);
module_exit(chardevicedriver_exit);
MODULE_LICENSE("GPL");
