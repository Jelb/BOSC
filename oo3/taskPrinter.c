#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>

//int count;

int taskprinter_entry(void)
{
  struct task_struct *task;
  //count = 0;
  printk(KERN_INFO "ID:      Name:               State:");
  for_each_process(task) {
    //count = count + 1;
    printk( KERN_INFO "%6i%20s%10ld\n", task->pid, task->comm, task->state);
  }
  //printk(KERN_INFO "Processes count: %i\n", count);
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
