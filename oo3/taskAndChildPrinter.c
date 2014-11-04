#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>

int taskprinter_entry(void)
{
  //Struct kan findes i sched.h l.1042
  struct task_struct *mainTask, *subTask;
  struct list_head *list;
  
  printk(KERN_INFO "ID:      Name:               State:");
  for_each_process(mainTask) {
    printk( KERN_INFO "%6i%20s%10ld\n", mainTask->pid, mainTask->comm, mainTask->state);
    printk( KERN_INFO "CHILD PROCESSES\n");
    list_for_each(list, &mainTask->children) {
      subTask = list_entry(list, struct task_struct, sibling); 
      printk( KERN_INFO "%6i%20s%10ld\n", subTask->pid, subTask->comm, subTask->state);
    }
    printk(KERN_INFO "------------------------------");
  }
  return 0;
}


void taskprinter_exit(void) 
{
  printk(KERN_INFO "------[Removing Module]-----\n");
}


module_init( taskprinter_entry );
module_exit( taskprinter_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Printer");
MODULE_AUTHOR("SGG");
