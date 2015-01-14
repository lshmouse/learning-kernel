##


### Install
```
sudo apt-get install systemtap
```

If you encounter the problem "missing debuginfo", please reference the blog at
http://www.cnblogs.com/hdflzh/archive/2012/07/25/2608910.html
Or
http://askubuntu.com/questions/197016/how-to-install-a-package-that-contains-ubuntu-kernel-debug-symbols

### Tests
```
stap -v -e 'probe vfs.read {printf("read performed\n"); exit()}'. 
```

### Reference
- https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/SystemTap_Beginners_Guide/

