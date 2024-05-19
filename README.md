# LinuxKernelModule
simple LKM development...

## 1. hello from kernel
主要是获得从内核的反馈和一些命令行的使用
```bash
git clone https://github.com/BriceLucifer/LinuxKernelModule.git
cd LinuxKernelModule
cd hello
make
```
编译结束之后
会看到mymodule.ko 这个就是待会要用导入到kernel的文件
在添加之前 先需要下载一些组件
```bash
# 我使用的是ubuntu 18.04 在这版本之上貌似都可以
sudo apt-get update
sudo apt-get install build-essential git libncurses5-dev libssl-dev flex bison gcc-12
```
安装之后
```bash
sudo insmod hello.ko # 添加模块
sudo dmesg # 不加sudo 好像也行
dmesg | tail 
```
然后就可以看到hello from kernel
**ps: 有个小坑 建议各位使用虚拟机去完成开发操作 UEFI模式下的物理机需要先关闭安全模式 如果只是学习 强烈建议使用虚拟机去开发 避免浪费时间 也不要用wsl 会报错**
```bash
sudo rmmod hello # 删除模块
dmesg | tail 
```
会看到 Goodbye kernel

---

## dev_nr
在完成hello之后
```bash
cd dev_nr
make 
```
编译之后
你就可以导入dev_nr.ko
```bash
sudo insmod dev_nr.ko
dmesg | tail
```
查看内核信息 在内核之后就可以查看所写的设备文件
可以使用```cat /proc/devices | grep dev_nr```查看设备文件的主编号和小编号 这里选择了 **90 0**

然后使用```sudo mknod /dev/mydevice c 90 0```创建设备文件

查看/dev/mydevice
```bash
ls -al /dev/mydevice
```
然后编写一个test.c文件 内容如下：
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	int ret = open("/dev/mydevice",O_RDONLY);
	if(ret == -1){
		printf("can not open sorry\n");
		return -1;
	}else{
		printf("open is successful\n");
	}
	close(ret);
	return 0;
}
```
之后继续使用命令行
``` bash
dmesg | tail -al # 查看状态
```
对于之前写的test.c文件 使用```gcc test.c -o test``` gcc编译之后用于检测 device_open是否正常使用
在使用**./test** 之前 我们先更改一下*/dev/mydevice*的权限
```bash
sudo chmod +666 /dev/mydevice
# 之后使用./test
./test
# 再使用dmesg
dmesg | tail -n 2
```
如果需要删除模块
```bash
sudo rmmod dev_nr
```
---

## 3_read_and_write
停更一下 期末考试。。。