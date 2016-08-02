#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <linux/seq_file.h>	/* for seq_file */
#include <linux/uaccess.h>

#define SIMPLE_READWRITE 	"simple"

MODULE_AUTHOR("Philippe Reynes");
MODULE_LICENSE("GPL");

static unsigned long counter = 0;
static char simple[128] = "Tristan Xiao's hello world";
#if 0
static char hello[128] = "Tristan Xiao's hello world";
/**
 * This function is called at the beginning of a sequence.
 * ie, when:
 *	- the /proc file is read (first time)
 *	- after the function stop (end of sequence)
 *
 */
static void *my_seq_start(struct seq_file *s, loff_t *pos)
{

	/* beginning a new sequence ? */	
	if ( *pos == 0 )
	{	
		/* yes => return a non null value to begin the sequence */
		return &counter;
	}
	else
	{
		/* no => it's the end of the sequence, return end to stop reading */
		*pos = 0;
		return NULL;
	}
}

/**
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 *
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v) += 3;
	(*pos)++;
	return NULL;
}

/**
 * This function is called at the end of a sequence
 * 
 */
static void my_seq_stop(struct seq_file *s, void *v) {
	/* nothing to do, we use a static value in start() */
}

/**
 * This function is called for each "step" of a sequence
 *
 */
static int my_seq_show(struct seq_file *s, void *v)
{
	/*
	loff_t *spos = (loff_t *) v;

	char a[4] = {};

	memcpy(a, hello , 3);
	*/
	seq_printf(s, "%s\n", hello);
	return 0;
}

/**
 * This structure gather "function" to manage the sequence
 *
 */
static struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next  = my_seq_next,
	.stop  = my_seq_stop,
	.show  = my_seq_show
};

static int my_simple_open(struct inode *inode, struct file *file)
{
	//return seq_open(file, &my_seq_ops);
	return single_open(file, simple_show, NULL);
};


static struct file_operations my_file_ops = {
	.owner   = THIS_MODULE,
	.open    = my_simple_open,
	.write	 = simple_write,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};
#endif

static int simple_show(struct seq_file *s, void *v)
{
	seq_printf(s, "%s\n", simple);
	return 0;
}

static int my_simple_open(struct inode *inode, struct file *file)
{
	//return seq_open(file, &my_seq_ops);
	return single_open(file, simple_show, NULL);
};

static ssize_t simple_write(struct file *file, const char __user *buf,
		                                   size_t count, loff_t *ppos)
{
	if (count) {
		if (copy_from_user(simple, buf, count)) {
			return -EFAULT;
		}
	}
	return count;
}
/**
 * This structure gather "function" that manage the /proc file
 *
 */
static struct file_operations my_simple_file_ops = {
	.owner   = THIS_MODULE,
	.open    = my_simple_open,
	.write	 = simple_write,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

/**
 * This function is called when the module is loaded
 *
 */
int init_module(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create(SIMPLE_READWRITE, 0, NULL, &my_simple_file_ops);

	return 0;
}

/**
 * This function is called when the module is unloaded.
 *
 */
void cleanup_module(void)
{
	remove_proc_entry(SIMPLE_READWRITE, NULL);
}

