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
#include <linux/semaphore.h>

struct class *char_class;
#define DEVICE_NAME "chardev"
#define CHARMAJOR 300
#define CHARMINOR 0
//对幻数的编号千万不能重复定义，如ioctl-number.txt
//_IO(type,nr)定义无参数的命令
#define MISC_IOC_MAGIC 'M'
#define LED_ON  _IO(MISC_IOC_MAGIC, 0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC, 0x02)

struct semaphore sem_open; //count semaphore
struct semaphore sem_read; //mutex semaphore
struct semaphore sem_write; //synchronous semaphore

static int chardevicedriver_open(struct inode *inode, struct file *filp){
	//try to get semaphore sem_open
	if(down_trylock(&sem_open)){
		printk("CharDeviceDriver: device can be open only 2 times\n");
		return -EBUSY;
	}
	printk("Success to get semaphore sem_open for opening device\n");

	return 0;
}

static ssize_t chardevicedriver_read(struct file *filp, char __user *buf,
			size_t count, loff_t *f_pos){
	//try to get semaphore sem_read 
	down_interruptible(&sem_read);
	printk("Success to get semaphore sem_read for reading device\n");

	return count;
}

static ssize_t chardevicedriver_write(struct file *filp, const char __user *buf,size_t count, loff_t *f_pos){
	
	//try to get semaphore sem_write
	if(down_trylock(&sem_write)){
		printk("device is waiting for something\n");
		return -EBUSY;
	}
	printk("Success to get semaphore sem_write for writting device\n");

	return 0;
}

static int chardevicedriver_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg){

	//up semaphore sem_read & sem_write only for demo
	up(&sem_read);
	up(&sem_write);

	return 0;
}

static int chardevicedriver_release(struct inode *inode, struct file *filp){
	
	up(&sem_open);
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
	//初始化信号量变量
	sema_init(&sem_open,2);//初始化信号量,并初始化值为2
	//init_MUTEX(&sem_read);//初始化sem_read为1 init_MUTEX 被废除
	sema_init(&sem_read,1);//初始化sem_read为1
	//init_MUTEX_LOCKED(&sem_write);//初始化sem_write值为0,并处于锁定状态
	sema_init(&sem_write,0);//初始化sem_write值为0,并处于锁定状态

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
