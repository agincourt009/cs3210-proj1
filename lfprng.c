#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

struct process_id{
    int pid;
    struct list_head current_thread;
    long long seed;
    boolean updated;
}

struct thread_id
{
	int tid;     // This is tid the way you have it. To get the equivalent of tgid in their code, add a head pointer to the process_id struct
	long long seed;
	struct list_head next_thread;
}

static struct proc_dir_entry *proc_entry;

static int lfprng_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{  
	long long val = getRandNumber(current->tgid, current->pid, procID.seed);

	int length = sprintf(page, "%lld", val);

  	return length;
}//end lfprng_read_proc function

static int lfprng_write_proc(struct file *file, const char *buf, usingned long count, void *data)
{
    	static const int SEED_SIZE = sizeof(long long);
	long long seed;

	if(count> SEED_SIZE)
	{
		count = SEED_SIZE;
	}//end if statement
        if(copy_from_user(&seed, buf, count))
    	{
    		return -EFAULT;
    	}
	setSeed(seed, current->tgid);
	
	return count;
}//end lfprng_write_proc function

void create_lfprng(){
}

void calc_lfprng(){
}

void create_New_Proc(int pid)//don't need to allocate memory for every thread. Just the first one, and then the rest will be handled by linked list
{
	procID.pid = pid;
	procID.current_thread = newProc_first_thread;
	for(rest of threads in newProc)
	{
		//add thread and it’s information
	}
	newProc.updated = true;
}//end create_New_Proc function

void seed(){
//	get thread number => check what is omp_get_thread_num and omp_get_num_threads
//	if no seed input, pick a random seed
//	set each thread a seed(seedn2 = seedn1*mult % PMOD)
//	figure out random last
}

static int __init lfprng_module_init(void){
	int rv = 0;
	proc_entry = create_proc_entry("lfprng", 0666, NULL);
	if(proc_entry == NULL){
		rv = -ENOMEN
		printk(KERN_INFO "lfprng: Couldn't create proc entry\n");
	}else{
		proc_entry->owner = THIS_MODULE;
		proc_entry->read_proc = lfprng_read_proc;
		proc_entry->write_proc = lfprng_write_proc;
		printk(KERN_INFO "lfprng_module_init called. Module now loaded.\n");
	}
	return rv;
}

static void __exit lfprng_module_cleanup(void){
	remove_proc_entry(lfprng, proc_entry);
	printK(KERN_INFO "lfprng_module_cleanup called. Module unloaded");
}

module_init(lfprng_module_init);
module_exit(lfprng_module_exit);

