/*************************************************************************
	> File Name: devicemodelkobject.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年06月01日 星期五 15时00分53秒
	> echo "test" > /sys/devicemodelkobject/devicemodelkobject_name"
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>

static struct kobject dmkobj;

static ssize_t devicemodelkobject_attr_show(struct kobject *kobj,
			struct attribute *attr, char *buf){
	return sprintf(buf,"Attribute Name show:%s\n", attr->name);
}

static ssize_t devicemodelkobject_attr_store(struct kobject *kobj,
			struct attriubte *attr, const char *buf, size_t count){
	char sbuf[100];
	sscanf(buf, "%s\n", sbuf);
	printk("DeviceModelKobject:Attribute Name store:%s\n", sbuf);

	return count;
}

static struct sysfs_ops devicemodelkobject_sysfs_ops = {
	.show = devicemodelkobject_attr_show,
	.store = devicemodelkobject_attr_store,
};

static void devicemodelkobject_release(struct kobject *kobj){

}

static struct attribute devicemodelkobject_attr_name = {
	.name = "devicemodelkobject_name",
	.mode = 0600,
};

static struct attribute *devicemodelkobject_default_attrs[] = {
	&devicemodelkobject_attr_name,
	NULL,
};

static struct kobj_type dmkobj_type = {
	.sysfs_ops = &devicemodelkobject_sysfs_ops,
	.release = devicemodelkobject_release,
	.default_attrs = devicemodelkobject_default_attrs,
};

static int __init devicemodelkobject_init(void){
	//完成对dmkobj对象的初始化
	kobject_init_and_add(&dmkobj, &dmkobj_type, NULL, "devicemodelkobject");
	return 0;
}

static void __exit devicemodelkobject_exit(void){
	kobject_put(&dmkobj);//减少引用计数,当引用计数为0时,所有该对象使用的资源将被释放

}

module_init(devicemodelkobject_init);
module_exit(devicemodelkobject_exit);
MODULE_LICENSE("GPL");

