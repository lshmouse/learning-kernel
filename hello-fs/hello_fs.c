#include <linux/module.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/highuid.h>
#include <linux/vfs.h>
#include <linux/writeback.h>

static uint32_t const HELLO_FS_MAGIC = 0x13131313;

static void hello_fs_put_super(struct super_block *sb)
{
  printk(KERN_INFO "Hello fs super block destroyed\n");
}

static struct super_operations const hello_fs_super_ops = {
  .put_super = hello_fs_put_super,
};

static int hello_fs_fill_super(struct super_block *sb, void *data, int silent)
{
  struct inode *root = NULL;
  sb->s_magic = HELLO_FS_MAGIC;
  sb->s_op = &hello_fs_super_ops;

  root = new_inode(sb);
  if (!root)
  {
    printk(KERN_ALERT "inode allocation failed\n");
    return -ENOMEM;
  }

  root->i_ino = 0;
  root->i_sb = sb;
  root->i_atime = root->i_mtime = root->i_ctime = CURRENT_TIME;
  inode_init_owner(root, NULL, S_IFDIR);

  sb->s_root = d_make_root(root);
  if (!sb->s_root)
  {
    printk(KERN_ALERT "root creation failed\n");
    return -ENOMEM;
  }

  return 0;
}

static struct dentry *hello_fs_mount(struct file_system_type *fs_type,
    int flags, const char *dev_name, void *data)
{
  printk(KERN_ALERT "Hello fs mounted\n");
  return mount_bdev(fs_type, flags, dev_name, data, hello_fs_fill_super);
}

static struct file_system_type hello_fs_type = {
  .owner		= THIS_MODULE,
  .name		= "hello_fs",
  .mount		= hello_fs_mount,
  .kill_sb	= kill_block_super,
  .fs_flags	= FS_REQUIRES_DEV,
};

static int __init hello_fs_init(void)
{
  printk(KERN_ALERT "Register hello fs\n");
  return register_filesystem(&hello_fs_type);
}

static void __exit hello_fs_exit(void)
{
  printk(KERN_ALERT "Unregister hello fs\n");
  unregister_filesystem(&hello_fs_type);
}

module_init(hello_fs_init);
module_exit(hello_fs_exit);

MODULE_ALIAS_FS("hello");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shaohui Liu");
