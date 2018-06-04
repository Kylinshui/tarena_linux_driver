/*************************************************************************
	> File Name: devicemodule.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年06月01日 星期五 16时05分56秒
	> cat /sys/bus/dmb_bus/bus_name
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

static struct bus_attribute bus_attr_bus_name;
static struct bus_attribute bus_attr_bus_modalias;

ssize_t dmb_bus_name_show(struct bus_type *bus, char *buf){

	printk("%s enter\n",__func__);

	return sprintf(buf,"DeviceModelBus:Bus Name show:%s\n",bus_attr_bus_name.attr.name);

}

ssize_t dmb_bus_name_store(struct bus_type *bus, const char *buf, size_t count){
	char sbuf[100];
	printk("%s enter\n",__func__);
	sscanf(buf, "%s\n",sbuf);
	printk("DeviceModelBus:Bus Name Store:%s\n",sbuf);

	return count;
}

ssize_t dmb_bus_modalias_show(struct bus_type *bus, char *buf){
	printk("%s enter\n",__func__);

	return sprintf(buf,"Bus ModAlias Show:%s\n", bus_attr_bus_modalias.attr.name);

}

ssize_t dmb_bus_modalias_store(struct bus_type *bus, const char *buf,size_t count){
	char sbuf[100];
	printk("%s  enter\n",__func__);

	sscanf(buf, "%s\n", sbuf);
	printk("devicemodelbus: Bus ModAlias Store:%s\n",sbuf);

	return count;
}

//bus_name 和bus_create_file的第二个参数bus_attr_bus_name相对应
static BUS_ATTR(bus_name, 0600, dmb_bus_name_show, dmb_bus_name_store);
static BUS_ATTR(bus_modalias,0600,dmb_bus_modalias_show,dmb_bus_modalias_store);


static struct bus_type devicemodelbus_bus_type = {
	.name = "dmb_bus",
};

static int __init devicemodelbus_init(void){
	int ret = 0;
	//如果注册成功新的总线将被添加进系统,该总线可以在/sys/bus目录下找到
	ret = bus_register(&devicemodelbus_bus_type);
	if(ret < 0){
		printk("Failure to register bus:%s\n", devicemodelbus_bus_type.name);
		return ret;
	}

	printk("Success to register bus:%s\n", devicemodelbus_bus_type.name);
	//在/sys/下创建一个文件夹
	bus_create_file(&devicemodelbus_bus_type, &bus_attr_bus_name);
	bus_create_file(&devicemodelbus_bus_type, &bus_attr_bus_modalias);

	return ret;
}

static void __exit devicemodelbus_exit(void){
	bus_remove_file(&devicemodelbus_bus_type, &bus_attr_bus_name);
	bus_unregister(&devicemodelbus_bus_type);
}

module_init(devicemodelbus_init);
module_exit(devicemodelbus_exit);
MODULE_LICENSE("GPL");
