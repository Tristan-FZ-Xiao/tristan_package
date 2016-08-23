#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <linux/seq_file.h>	/* for seq_file */
#include <linux/uaccess.h>
#include <linux/slab.h>


MODULE_AUTHOR("Philippe Reynes");
MODULE_LICENSE("GPL");


#define CONFIG_SEQ_READ_WRITE
#define CONFIG_SIMPLE_READ_WRITE

#ifdef CONFIG_SEQ_READ_WRITE
#define SEQ_READWRITE 	"seq"
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
	if (*pos > 10)
		return NULL;
	return pos;
}

/**
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 *
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	if (*pos > 10)
		return NULL;
	return pos;
}

/**
 * This function is called at the end of a sequence
 * 
 */
static void my_seq_stop(struct seq_file *s, void *v) {
	/* nothing to do, we use a static value in start() */
	//kfree(v);
}

/**
 * This function is called for each "step" of a sequence
 *
 */
static int my_seq_show(struct seq_file *s, void *v)
{
	int i = *(loff_t*)v;

	//printk(KERN_ALERT"%s v %d\n", __func__, 1000);
	seq_printf(s, "%s\n", hello);
	//seq_putc(s, *(hello+i));
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

static int my_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &my_seq_ops);
}

static struct file_operations my_seq_file_ops = {
	.owner   = THIS_MODULE,
	.open    = my_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};
#endif

#ifdef CONFIG_SIMPLE_READ_WRITE
#define SIMPLE_READWRITE 	"simple"
static char simple[128] = "Tristan Xiao's hello world";
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
#endif

/**
 * This function is called when the module is loaded
 *
 */
int init_module(void)
{

#ifdef CONFIG_SIMPLE_READ_WRITE
	{
	struct proc_dir_entry *simple_entry;
	simple_entry = proc_create(SIMPLE_READWRITE, 0, NULL, &my_simple_file_ops);
	}
#endif

#ifdef CONFIG_SEQ_READ_WRITE
	{
	struct proc_dir_entry *seq_entry;
	seq_entry = proc_create(SEQ_READWRITE, 0, NULL, &my_seq_file_ops);
	}
#endif
	return 0;
}

/**
 * This function is called when the module is unloaded.
 *
 */
void cleanup_module(void)
{
#ifdef CONFIG_SIMPLE_READ_WRITE
	remove_proc_entry(SIMPLE_READWRITE, NULL);
#endif
#ifdef CONFIG_SEQ_READ_WRITE
	remove_proc_entry(SEQ_READWRITE, NULL);
#endif
}

