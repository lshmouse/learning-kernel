#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>

static int hello_proc_show(struct seq_file *m, void *v)
{
  seq_printf(m, "hello world\n");
  return 0;
}

static int hello_proc_open(struct inode *inode, struct file *file)
{
  return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
  .owner      = THIS_MODULE,
  .open       = hello_proc_open,
  .read       = seq_read,
  .llseek     = seq_lseek,
  .release    = single_release,
};

static int __init hello_proc_init(void)
{
  printk(KERN_INFO "Create /proc/hello_wolrd\n");
  proc_create("hello_world", 0, NULL, &hello_proc_fops);
  return 0;
}

static void __exit hello_proc_exit(void) {
  printk(KERN_INFO "Remove /proc/hello_wolrd\n");
  remove_proc_entry("hello_world", NULL);
}

module_init(hello_proc_init);
module_exit(hello_proc_exit);

MODULE_AUTHOR("Shaohui Liu");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A hello_world /proc file.");
