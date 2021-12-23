<h1 align = "center">浙江大学实验报告</h1>

课程名称： 操作系统
实验项目名称：Rinux环境搭建和内核编译
学生姓名： 学号：何瑞桓 3190101928
电子邮件地址：3190101928@zju.edu.cn
实验日期： 2021年9月23日

## 一、实验内容

### 1.1 搭建docker

打开新建的Ubuntu虚拟机环境，输入ctrl+alt+t打开命令行终端，输入以下指令完成docker安装

```bash
### 安装docker
$ sudo apt-get install curl
$ curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
### 将用户加入docker组，免sudo
$ sudo usermod -aG docker $USER
```

在学在浙大下载课程提供的oslab.tar文件，并放入主目录。执行以下指令将该docker镜像导入

```bash
`$ cat oslab.tar | docker import - oslab:2021`
```

输入以下指令查看当前存在的docker容器

```bash
$ docker image ls
```

![](Pictures\$}MSZP}0UC06{2]_5JKWIN7.png)

输入以下指令查看所有存在的容器

```bash
$ docker ps -a
```

![](Pictures\QQ截图20210925210200.png)

开启并进入容器

```bash
docker start ed0b ### ed0b 为容器id的前四位
docker exec -it -u oslab -w /home/oslab ed /bin/bash ### ed 为容器id的前两位
```

![](Pictures\QQ截图20210925210826.png)

### 1.2 编译Linux内核

```bash
### 进入实验目录并设置环境变量
# pwd
/home/oslab
# cd lab0
# export TOP=`pwd`
# export RISCV=/opt/riscv
# export PATH=$PATH:$RISCV/bin
# mkdir -p build/linux
# make -C linux O=$TOP/build/linux CROSS_COMPILE=riscv64-unknown-linux-gnu-
ARCH=riscv CONFIG_DEBUG_INFO=y defconfig all -j$(nproc)
```

编译产生文件如下![](Pictures\QQ截图20210925211829.png)

### 1.3 使用QEMU运行内核

输入以下命令运行该Linux内核

```bash
### 用户名root，没有密码
# qemu-system-riscv64 -nographic -machine virt -kernel
build/linux/arch/riscv/boot/Image \
-device virtio-blk-device,drive=hd0 -append "root=/dev/vda ro console=ttyS0"
\
-bios default -drive file=rootfs.ext4,format=raw,id=hd0 \
-netdev user,id=net0 -device virtio-net-device,netdev=net0
```

![](Pictures\QQ截图20210925213315.png)

![](Pictures\QQ截图20210925213437.png)

成功进入了界面并登陆了root账户

退出QEMU模拟器的方法为：使用`ctrl`+`a`(macOS下为`control+a`)，松开后再按下`x`键即可退出QEMU

### 1.4 使用 GDB 对内核进行调试

在通过QEMU运行的指令后加-S -s进入调试模式

![](Pictures\QQ截图20210925214710.png)

随后打开另一个终端，进入该docker容器，并运行GDB

```bash
# riscv64-unknown-linux-gnu-gdb build/linux/vmlinux
```

![](Pictures\QQ截图20210925215434.png)

连接QEMU

![](Pictures\QQ截图20210925215759.png)

输入 list 或 l 可以查看原代码

![](Pictures\QQ截图20210926140005.png)

后接数字或函数名可以跳到对应位置

![](Pictures\QQ截图20210926140923.png)

![](Pictures\QQ截图20210926143532.png)

输入 run/r 可以运行正在进行调试的程序

输入 breakpoint/b 后接函数名或行数可以在对应位置设置断点

![](Pictures\QQ截图20210926145615.png)

输入 continue 后运行到该程序的下一个断点

![](Pictures\QQ截图20210926145746.png)

info 后接各类信息名称可查看相关信息

![](Pictures\QQ截图20210926150243.png)

print 后接变量名可以查看当前该变量值

在单步调试中：

next / n 不进入的单步执行

step 进入的单步执行

finish 如果已经进入了某函数，而想退出该函数返回到它的调用函数中，可使用命令finish

until 结束当前循环

![](Pictures\QQ截图20210926150648.png)

kill 异常终止当前 gdb 控制下的程序

quit 退出 gdb

## 二、讨论、心得

​		本来觉得每次进入docker都要进行export修改环境变量过于麻烦，想要对环境变量进行永久修改，但修改docker下的 /etc/profile 并没有产生效果，后检查发现若在profile或bashrc文件中按顺序写入

```bash
export RISCV=/opt/riscv
export PATH=$PATH:$RISCV/bin
```

事实上第二个RISCV并不会被读作/opt/riscv，所以修改该文件时应该写作

```bash
export RISCV=/opt/riscv
export PATH=$PATH:/opt/riscv/bin
```

​		重启docker后，输入export查看环境变量，可以看到已经修改完成了。

![](Pictures\QQ截图20211005134641.png)

注：对环境变量的永久更改需要在root权限下进行，在运行docker的指令中将-u 后的 oslab 改为 0 即可以 root 账户运行docker。

​		本次实验总体来说还比较简单，按实验指导做就可以完成，docker的使用可以查阅相关文档和网上的教程进行。并没有遇到较大的阻碍和困难。

