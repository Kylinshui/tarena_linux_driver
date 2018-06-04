/*************************************************************************
	> File Name: virtualnetdevice.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年06月04日 星期一 13时32分46秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#define VIRTUALNETDEVICE_CARD_NAME "vndcard"
static struct net_device *virtualnetdevice_card;

static int virtualnetdevice_card_init(struct net_device *dev){
	printk("%s enture\n",__func__);

	ether_setup(dev);

	strcpy(dev->name, VIRTUALNETDEVICE_CARD_NAME);

	return 0;
}

static struct net_device *virtualnetdevice_open(struct net_device *dev){
	printk("%s enture\n",__func__);

	return 0;
}

static int virtualnetdevice_stop(struct net_device *dev){
	printk("%s enture\n",__func__);

	return 0;
}
//启动数据包的发送
static int virtualnetdevice_hard_start_xmit(struct sk_buff *skb,struct net_device *dev){
	printk("%s entry\n",__func__);

	dev_kfree_skb(skb);//释放内存

	return 0;
}

static const struct net_device_ops virt_netdev_ops = {
	.ndo_init = virtualnetdevice_card_init,
	.ndo_open = virtualnetdevice_open,
	.ndo_stop = virtualnetdevice_stop,
	.ndo_start_xmit = virtualnetdevice_hard_start_xmit,
};

static int __init virtualnetdevice_init(void){
	int ret = 0;
	//allocate and initialize_net_device
	virtualnetdevice_card = alloc_etherdev(0);

	virtualnetdevice_card->netdev_ops = &virt_netdev_ops;
	virtualnetdevice_card->dev_addr[0] = 0x08;
	virtualnetdevice_card->dev_addr[1] = 0x09;
	virtualnetdevice_card->dev_addr[2] = 0x0a;
	virtualnetdevice_card->dev_addr[3] = 0x0b;
	virtualnetdevice_card->dev_addr[4] = 0x00;
	virtualnetdevice_card->dev_addr[5] = 0x0c;

	//注册网络设备
	ret = register_netdev(virtualnetdevice_card);
	if(ret < 0){
		printk("Failure to register net device\n");
		return ret;
	}

	printk("VirtualNetDevice:Success to register net device:%s\n",VIRTUALNETDEVICE_CARD_NAME);

	return ret;
}

static void __exit virtualnetdevice_exit(void){
	unregister_netdev(virtualnetdevice_card);
}

module_init(virtualnetdevice_init);
module_exit(virtualnetdevice_exit);
MODULE_LICENSE("GPL");
