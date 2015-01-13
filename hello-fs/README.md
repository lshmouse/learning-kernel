## Write a hello file system based on memmory

### Steps
* Registering A New File-System

See: include/linux/fs.h 
```
struct file_system_type {
  const char *name;
  int fs_flags;
  struct super_block *(*mount) (struct super_block *, void *, int);
  void (*kill_sb) (struct super_block *);
  ...
};
```
Using register_filesystem/unregister_filesystem

* mount/umount File System

See mount_fs in fs/super.c  
```
struct dentry * mount_fs(struct file_system_type *type, int flags, const char *name, void *data) {
  ...
    root = type->mount(type, flags, name, data);
  ...
}
```

See deactivate_locked_super in fs/super.c
  ```
void deactivate_locked_super(struct super_block *s)                                                                                                                               
{                                                                                                        
  struct file_system_type *fs = s->s_type;                                                               
  if (atomic_dec_and_test(&s->s_active)) {                                                               
    cleancache_invalidate_fs(s);                                                                         
    unregister_shrinker(&s->s_shrink);                                                                   
    fs->kill_sb(s);                                                                                      

    put_filesystem(fs);                                                                                  
    put_super(s);                                                                                        
  } else {                                                                                               
    up_write(&s->s_umount);                                                                          
  }                                                                                                  
}                                                                                                    
  ```
* superblock operations

```
struct super_operations {                                                                            
    struct inode *(*alloc_inode)(struct super_block *sb);                                                
  void (*destroy_inode)(struct inode *);                                                                 
                                                                                                         
    void (*dirty_inode) (struct inode *, int flags);                                                     
  int (*write_inode) (struct inode *, struct writeback_control *wbc);                                    
  int (*drop_inode) (struct inode *);                                                                    
  void (*evict_inode) (struct inode *);                                                                  
  void (*put_super) (struct super_block *);                                                              
  int (*sync_fs)(struct super_block *sb, int wait);                                                      
  int (*freeze_super) (struct super_block *);                                                            
  int (*freeze_fs) (struct super_block *);                                                               
  int (*thaw_super) (struct super_block *);                                                              
  int (*unfreeze_fs) (struct super_block *);                                                             
  int (*statfs) (struct dentry *, struct kstatfs *);                                                     
  int (*remount_fs) (struct super_block *, int *, char *);                                               
  void (*umount_begin) (struct super_block *);                                                           
                                                                                                         
  int (*show_options)(struct seq_file *, struct dentry *);                                               
  int (*show_devname)(struct seq_file *, struct dentry *);                                               
  int (*show_path)(struct seq_file *, struct dentry *);                                                  
  int (*show_stats)(struct seq_file *, struct dentry *);                                                 
#ifdef CONFIG_QUOTA                                                                                      
  ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);                              
  ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);                       
  struct dquot **(*get_dquots)(struct inode *);                                                          
#endif                                                                                                   
  int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);                                
  long (*nr_cached_objects)(struct super_block *, int);                                                  
  long (*free_cached_objects)(struct super_block *, long, int);                                          
};         
```

* Inodes Operations

Inode opreations see include/linux/fs.h
```
struct inode_operations {                                                                            
  struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);                          
  void * (*follow_link) (struct dentry *, struct nameidata *);                                       
  int (*permission) (struct inode *, int);                                                           
  struct posix_acl * (*get_acl)(struct inode *, int);                                                
                                                                                                     
  int (*readlink) (struct dentry *, char __user *,int);                                              
  void (*put_link) (struct dentry *, struct nameidata *, void *);                                    
                                                                                                     
  int (*create) (struct inode *,struct dentry *, umode_t, bool);                                     
  int (*link) (struct dentry *,struct inode *,struct dentry *);                                      
  int (*unlink) (struct inode *,struct dentry *);                                                    
  int (*symlink) (struct inode *,struct dentry *,const char *);                                      
  int (*mkdir) (struct inode *,struct dentry *,umode_t);                                             
  int (*rmdir) (struct inode *,struct dentry *);                                                     
  int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);                                       
  int (*rename) (struct inode *, struct dentry *,                                                    
      struct inode *, struct dentry *);                                                              
  int (*rename2) (struct inode *, struct dentry *,                                                   
      struct inode *, struct dentry *, unsigned int);                                                
  int (*setattr) (struct dentry *, struct iattr *);                                                  
  int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);                            
  int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);                           
  ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);                               
  ssize_t (*listxattr) (struct dentry *, char *, size_t);                                            
  int (*removexattr) (struct dentry *, const char *);                                                
  int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,                              
          u64 len);                                                                                  
  int (*update_time)(struct inode *, struct timespec *, int);                                        
  int (*atomic_open)(struct inode *, struct dentry *,                                                
         struct file *, unsigned open_flag,                                                          
         umode_t create_mode, int *opened);                                                          
  int (*tmpfile) (struct inode *, struct dentry *, umode_t);                                         
  int (*set_acl)(struct inode *, struct posix_acl *, int);                                           
                                                                                                     
  /* WARNING: probably going away soon, do not use! */                                               
  int (*dentry_open)(struct dentry *, struct file *, const struct cred *);                           
} ____cacheline_aligned;  

```

* Dentries opreations

Dentries operations see include/dcache.h

```
struct dentry_operations {                                                                              
  int (*d_revalidate)(struct dentry *, unsigned int);                                                   
  int (*d_weak_revalidate)(struct dentry *, unsigned int);                                              
  int (*d_hash)(const struct dentry *, struct qstr *);                                                  
  int (*d_compare)(const struct dentry *, const struct dentry *,                                        
      unsigned int, const char *, const struct qstr *);                                                 
  int (*d_delete)(const struct dentry *);                                                               
  void (*d_release)(struct dentry *);                                                                   
  void (*d_prune)(struct dentry *);                                                                     
  void (*d_iput)(struct dentry *, struct inode *);                                                      
  char *(*d_dname)(struct dentry *, char *, int);                                                       
  struct vfsmount *(*d_automount)(struct path *);                                                       
  int (*d_manage)(struct dentry *, bool);                                                               
} ____cacheline_aligned;                                                                                
   
```
* Files

Files operations see include/linux/fs.h
```
struct file_operations {                                                                                 
  struct module *owner;                                                                                  
  loff_t (*llseek) (struct file *, loff_t, int);                                                         
  ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);                                      
  ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);                               
  ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);                     
  ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);                    
  ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);                                              
  ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);                                             
  int (*iterate) (struct file *, struct dir_context *);                                                  
  unsigned int (*poll) (struct file *, struct poll_table_struct *);                                      
  long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);                                   
  long (*compat_ioctl) (struct file *, unsigned int, unsigned long);                                     
  int (*mmap) (struct file *, struct vm_area_struct *);                                                  
  void (*mremap)(struct file *, struct vm_area_struct *);                                                
  int (*open) (struct inode *, struct file *);                                                           
  int (*flush) (struct file *, fl_owner_t id);                                                           
  int (*release) (struct inode *, struct file *);                                                        
  int (*fsync) (struct file *, loff_t, loff_t, int datasync);                                            
  int (*aio_fsync) (struct kiocb *, int datasync);                                                       
  int (*fasync) (int, struct file *, int);                                                               
  int (*lock) (struct file *, int, struct file_lock *);                                                  
  ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);                        
  unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
  int (*check_flags)(int);                                                                               
  int (*flock) (struct file *, int, struct file_lock *);                                                 
  ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);  
  ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);   
  int (*setlease)(struct file *, long, struct file_lock **, void **);                                    
  long (*fallocate)(struct file *file, int mode, loff_t offset,                                          
        loff_t len);                                                                                     
  void (*show_fdinfo)(struct seq_file *m, struct file *f);                                               
}; 
```

### Reference
- http://haifux.org/lectures/120/writing-linux-2.4-fs/writing-linux-2.4-fs.html
- http://kukuruku.co/hub/nix/writing-a-file-system-in-linux-kernel
- http://www.win.tue.nl/~aeb/linux/lk/lk-8.html
