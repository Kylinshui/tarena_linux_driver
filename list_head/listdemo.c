/*************************************************************************
	> File Name: listdemo.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月25日 星期五 16时33分05秒
	> 实现链表的创建,增加节点,遍历操作
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#define EMPLOYEE_NUMBEROF	5

static struct list_head employee_list;

struct employee{
	char name[20];
	int number;
	int salary;
	struct list_head list;
};

static struct employee *employeep = NULL;
static struct employee *employee_temp = NULL;
static struct list_head *pos = NULL;


static int __init listdemo_init(void){
	int i;
	printk("Entry listdemo init\n");
	INIT_LIST_HEAD(&employee_list);

	//allocate memory for employee array
	employeep = kmalloc(sizeof(struct employee)*EMPLOYEE_NUMBEROF,GFP_KERNEL);
	if(IS_ERR(employeep)){
		printk("Cann't allocate memory for employee\n");
		return -ENOMEM;
	}

	memset(employeep, 0, sizeof(struct employee)*EMPLOYEE_NUMBEROF);

	//fill data
	for(i=0; i< EMPLOYEE_NUMBEROF;i++){
		sprintf(employeep[i].name,"Employee%d",i+1);
		employeep[i].number = 1001+i;
		employeep[i].salary = 5000;
		list_add(&(employeep[i].list),&employee_list);
	}

	printk("Using list_for_each print all information of members\n");
	list_for_each(pos, &employee_list){
		employee_temp = list_entry(pos, struct employee,list);

		printk("Employee Name: %s\t Number:%d\t Salary:%d\t \n",
					employee_temp->name,
					employee_temp->number,
					employee_temp->salary);
	}

	return 0;
}

static void __exit listdemo_exit(void){
	int i;
	for(i=0; i< EMPLOYEE_NUMBEROF;i++){
		list_del(&employeep[i].list);
	}

	kfree(employeep);
}

module_init(listdemo_init);
module_exit(listdemo_exit);
MODULE_LICENSE("GPL");
