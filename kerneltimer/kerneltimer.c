/*************************************************************************
	> File Name: kerneltimer.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 09时37分35秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

#define BARK_TIME 5 //time interval
#define BARK_TIMES 100//bark times

static struct timer_list bigdog_timer;
static unsigned long bark_time  = BARK_TIME;
static unsigned long bark_times = BARK_TIMES;
static int bark_count = 0;


static void bigdog_timer_handler(unsigned long data){
	printk("Entry bigdog_timer_handler\n");

	if(bark_count < bark_times){
		printk("Bigdog barking times:%d\n", ++bark_count);
		//修改定时器
		mod_timer(&bigdog_timer,jiffies+(bark_time*HZ));
	}
}

static int __init kerneltimer_init(void){
	init_timer(&bigdog_timer);
	bigdog_timer.expires = jiffies + (bark_time*HZ);
	bigdog_timer.function = bigdog_timer_handler;

	//add timer_list to kernel 
	add_timer(&bigdog_timer);

	return 0;
}

static void __exit kerneltimer_exit(void){
	del_timer(&bigdog_timer);
}

module_init(kerneltimer_init);
module_exit(kerneltimer_exit);
MODULE_LICENSE("GPL");
