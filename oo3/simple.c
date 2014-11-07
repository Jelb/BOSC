#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct birthday { 
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

void create(int day, int month, int year, struct birthday *person);

/* This function is called when the module is loaded. */
int simple_init(void)
{
  struct birthday *person;
  int i;
  printk(KERN_INFO "------[Loading Module]------\n");

  for (i = 0; i < 5; i++) {
    person = kmalloc(sizeof(*person), GFP_KERNEL);
    person->day = ((i+1)*100)%31;
    person->month = ((i*i)%12)+1;
    person->year = 2000 + i*i;
    INIT_LIST_HEAD(&person->list);
    printk(KERN_INFO "Adding birthday (%2d:%2d:%4d)\n", person->day, person->month, person->year);
    /* ADDING ELEMENT TO LIST */
    list_add_tail(&person->list, &birthday_list);
  }
  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
  struct birthday *ptr, *next;
  printk(KERN_INFO "------[Removing Module]-----\n");
  
  list_for_each_entry_safe(ptr, next, &birthday_list, list) { 
    /* on each  iteration ptr points */
    /* to the next birthday struct */
    printk(KERN_INFO "Removing birthday (%2d:%2d:%4d)\n", ptr->day, ptr->month, ptr->year);
    list_del(&ptr->list);
    kfree(ptr);
  }
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");