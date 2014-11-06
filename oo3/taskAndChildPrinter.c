#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>

//int count;

void dfs(struct task_struct *parentTask, int generation)
{
  struct list_head *list;
  generation = generation + 1;
    
  list_for_each(list, &parentTask->children)
  {
    struct task_struct *childTask;
    //count = count + 1;
    childTask = list_entry(list, struct task_struct, sibling); 
    printk(KERN_INFO "%6i%9i%5i%19s%7ld\n", childTask->pid, parentTask->pid, generation, childTask->comm, childTask->state);
    dfs(childTask,generation);
  }
}

int taskprinter_entry(void)
{
  struct task_struct *task;
  int generation;
  generation = 0;
  //count = 0;
  
  for_each_process(task)
  {
    //count = count + 1;
    printk(KERN_INFO "    ID   PARENT  GEN               NAME  STATE");
    printk(KERN_INFO "%6i%9s%5i%19s%7ld\n", task->pid, "N/A", generation, task->comm, task->state);
    dfs(task, generation);
    printk(KERN_INFO "----------------------------------------------\n");
  }
  //printk(KERN_INFO "Processes count: %i\n", count);
  return 0;
}

void taskprinter_exit(void) 
{
  printk(KERN_INFO "---------------[Removing Module]--------------\n");
}

module_init( taskprinter_entry );
module_exit( taskprinter_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Printer");
MODULE_AUTHOR("SGG");