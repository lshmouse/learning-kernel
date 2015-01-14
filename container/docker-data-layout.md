## Docker
Docker主要使用LXC(linux contianer)实现虚拟化

###Data layout
Images:
  /var/lib/docker/aufs/diff/${image_id}, Image也是aufs的一层layer

Docker启动之后的rootfs：
  /var/lib/docker/aufs/mnt/${containner_id}

The topest layer:
  /var/lib/docker/aufs/diff/${containner_id}

Container使用与宿主机器相同的内核：
```
# uname -r
3.13.0-36-generic
# sudo docker run -i -t ubuntu /bin/bash 
root@b8d1d6a9465b:/# uname -r
3.13.0-36-generic
```

```
# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED              STATUS
PORTS               NAMES
b8d1d6a9465b        ubuntu:12.04        /bin/bash           About a minute ago   Up About a minute evil_poincare   

# docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
ubuntu              12.04               8dbd9e392a96        21 months ago       128 MB
```

Layer 0: the image
```
# ls /var/lib/docker/aufs/diff/8dbd9e392a964056420e5d58ca5cc376ef18e2de93b5cc90e868a1bbc8318c1c/
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  selinux  srv  sys  tmp  usr  var
```
Layer 1: 

```
# ls /var/lib/docker/aufs/diff/b8d1d6a9465b19af6329eb34a3cb3b06bddfd80c60a4026f76478fc44fa8f83d
dev  etc  lib  usr  var
```

Container Rootfs: The union of layer 0 and layer 1
```
# ls /var/lib/docker/aufs/mnt/b8d1d6a9465b19af6329eb34a3cb3b06bddfd80c60a4026f76478fc44fa8f83d
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  selinux  srv  sys tmp  usr  var
```

Layer Structure:

存储在:/var/lib/docker/aufs/layer/${container_id}
```
# cat /var/lib/docker/aufs/layer/b8d1d6a9465b19af6329eb34a3cb3b06bddfd80c60a4026f76478fc44fa8f83d
b8d1d6a9465b19af6329eb34a3cb3b06bddfd80c60a4026f76478fc44fa8f83d-init
8dbd9e392a964056420e5d58ca5cc376ef18e2de93b5cc90e868a1bbc8318c1c
```

创建Image
```
# docker commit 8615197b12e6 ubuntu:12.04-mi
6fe0c0aaadaad82770f5ac54b67209f8ec1afb8621f2e8354644c3712e5ad0ca

#ls /var/lib/docker/graph/6fe0c0aaadaad82770f5ac54b67209f8ec1afb8621f2e8354644c3712e5ad0ca/
json  layersize

#cat /var/lib/docker/graph/6fe0c0aaadaad82770f5ac54b67209f8ec1afb8621f2e8354644c3712e5ad0ca/json | python -m json.tool
{
    ...
    "os": "linux",
    "parent": "8dbd9e392a964056420e5d58ca5cc376ef18e2de93b5cc90e868a1bbc8318c1c"
}

#ls /var/lib/docker/aufs/diff/6fe0c0aaadaad82770f5ac54b67209f8ec1afb8621f2e8354644c3712e5ad0ca/
dev  etc  lib  usr  var

```

Using chroot
```
chroot /var/lib/docker/aufs/mnt/489e9d9962f8e91ac691db5f80a51ec383d62694bcea110a4f901c0ae4696d1c bash
root@liushaohui-OptiPlex-990:/# ls
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  selinux  srv  sys tmp  usr  var
root@liushaohui-OptiPlex-990:/# uname -r
3.13.0-36-generic
```
