#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aaa");
MODULE_DESCRIPTION("a fs number for device and some callback functions");

// open() function for device open
static int device_open(struct inode *device_file, struct file* instance){
        printk("dev_nr open function is called!!\n");
        return 0;
}

// close() function for device close
static int device_close(struct inode *device_file, struct file* instance){
        printk("dev_nr close function is called!!\n");
        return 0;
}

// struct contains some owner open release
static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = device_open,
        .release = device_close
};

// 90 定义设备端口为90
#define MYMAJOR 90

static int __init ModuleInit(void){
        int retval;
        printk("hello from kernel!\n");
        retval = register_chrdev(MYMAJOR,"my_dev_nr",&fops);
        if (retval == 0){
                printk("dev_nr device number is %d,minor number is %d\n",MYMAJOR,0);
        }else if (retval > 0){
                printk("dev_nr device number is %d,minor number is %d\n",retval>>20,retval&0xfffff);
        }else{
                printk("can not register a device number!\n");
                return -1;
        }

        return 0;
}

static void __exit ModuleExit(void){
        unregister_chrdev(MYMAJOR,"my_dev_nr");
        printk("GoodBye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);