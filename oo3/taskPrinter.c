#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>

int taskprinter_entry(void)
{
  //Struct kan findes i sched.h l.1042
  struct task_struct *task;
  printk(KERN_INFO "ID:      Name:               State:");
  for_each_process(task) {
    printk( KERN_INFO "%6i%20s%10ld\n", task->pid, task->comm, task->state);
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
