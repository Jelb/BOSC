#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "birthday.h"


static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
	struct birthday *person1, *person2, *person3, *person4, *person5;
	int i;
	struct birthday *ptr;

	printk(KERN_INFO "Loading Module\n");

	person1 = kmalloc(sizeof(*person1), GFP_KERNEL);
	person1->day = 2;
	person1->month = 8;
	person1->year = 1995;
	INIT_LIST_HEAD(&person1->list);

	person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
	person2->day = 4;
	person2->month = 5;
	person2->year = 1992;

	person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
	person3->day = 1;
	person3->month = 11;
	person3->year = 1991;

	person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
	person4->day = 17;
	person4->month = 2;
	person4->year = 1968;

	person5 = kmalloc(sizeof(*person5), GFP_KERNEL);
	person5->day = 6;
	person5->month = 6;
	person5->year = 1979;

	list_add_tail(&person1->list, &birthday_list);
	list_add_tail(&person2->list, &birthday_list);
	list_add_tail(&person3->list, &birthday_list);
	list_add_tail(&person4->list, &birthday_list);
	list_add_tail(&person5->list, &birthday_list);

	i = 0;

	/* @ptr:			the type * to use as a loop cursor
	 * @birthday_list:	the head for your list
	 * @list:			the name of the list_struct within the struct
	 */
	list_for_each_entry(ptr, &birthday_list, list) {
	/* on each iteration ptr points */
	/* to the next birthday struct */
		i = i + 1;
		printk(KERN_INFO "element:%d, %d/%d/%d\n", i, ptr->day, ptr->month, ptr->year);
	}

	//printk(KERN_INFO "elements in list: %d\n", i);

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	struct birthday *ptr, *next;
	int i;

	i = 0;

	printk(KERN_INFO "Removing Module\n");

	/*
	* @ptr:				the type * to use as a loop cursor
	* @next:			another type * to use as a temporary storage
	* @birthday_list:	the head of the list
	* @list:			the name of the list_struct within the struct
	*/


	printk(KERN_INFO "status before remove process:\n");
	list_for_each_entry(ptr, &birthday_list, list) {
	/* on each iteration ptr points */
	/* to the next birthday struct */
		i = i + 1;
		printk(KERN_INFO "element:%d, %d/%d/%d\n", i, ptr->day, ptr->month, ptr->year);
	}

	i = 0;

	printk(KERN_INFO "removing...\n");

	list_for_each_entry_safe(ptr,next,&birthday_list,list) {
		/* on each iteration ptr points */
		/* to the next birthday struct */
		list_del(&ptr->list);
		kfree(ptr);
	}

	list_for_each_entry(ptr, &birthday_list, list) {
	/* on each iteration ptr points */
	/* to the next birthday struct */
		i = i + 1;
		printk(KERN_INFO "element:%d, %d/%d/%d\n", i, ptr->day, ptr->month, ptr->year);
	}

	printk(KERN_INFO "done removing\n");

	kfree(&birthday_list);

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

