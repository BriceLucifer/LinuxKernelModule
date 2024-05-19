#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/*meta data*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("aaa");
MODULE_DESCRIPTION("read write");

// buffer
static char buffer[255];
static int buffer_pointer;

// varibale for device and device class
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "DummuyDriver"
#define DRIVER_CLASS "DummyDriverClass"

/*
 * @brief read data out of buffer
 * */
static ssize_t driver_read(struct file *File,char *user_buffer,size_t count, loff_t *offs){
        int to_copy, not_copied, delta;
        // get data
        to_copy = min(count,buffer_pointer);

        //copy data to user
        not_copied = copy_to_user(user_buffer,buffer,to_copy);

        // calculate data
        delta = to_copy - not_copied;

        return delta;
}

static ssize_t driver_write(struct file *File,const char *user_buffer,size_t count, loff_t *offs){
        int to_copy, not_copied, delta;
        // get data
        to_copy = min(count,sizeof(buffer));

        //copy data to user
        not_copied = copy_from_user(buffer, user_buffer, to_copy);
        buffer_pointer = to_copy;
        // calculate data
        delta = to_copy - not_copied;

        return delta;
}


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
        .release = device_close,
        .read = driver_read,
        .write = driver_write
};

static int __init ModuleInit(void){
        printk("hello from kernel!\n");

        if( alloc_chrdev_region(&my_device_nr, 0, 1,DRIVER_NAME) < 0 ){
                printk("Device Nr . could not be allocated!\n");
                return -1;
        }

        printk("read_write - Device Nr. Majot :%d, Minor :%d was registered!\n",my_device_nr >> 20 , my_device_nr&0xfffff);
        // create device class
        if((my_class = class_create(THIS_MODULE,DRIVER_CLASS)) == NULL){
                printk("Device class can not create!\n");
                goto ClassError;
        }

        if(device_create(my_class,NULL,my_device_nr,NULL, DRIVER_NAME) == NULL){
                printk("can not create device file!\n");
                goto FileError;
        }

        // initialize device file
        cdev_init(&my_device,&fops);

        // Register device to kernel
        if(cdev_add(&my_device,my_device_nr,1) == -1){
                printk("Register of device to kernel failed!\n");
                goto AddError;
        }
        return 0;

AddError:
        device_destroy(my_class, my_device_nr);
FileError:
        class_destroy(my_class);
ClassError:
        unregister_chrdev_region(my_device_nr, 1);
        return -1;
}

static void __exit ModuleExit(void){
        cdev_del(&my_device);
        device_destroy(my_class, my_device_nr);
        class_destroy(my_class);
        unregister_chrdev(my_device_nr,DRIVER_NAME);
        printk("GoodBye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);