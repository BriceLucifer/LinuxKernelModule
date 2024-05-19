#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aaa");
MODULE_DESCRIPTION("Hello form kernel");

static int __init ModuleInit(void){
        printk("hello from kernel!\n");
        return 0;
}

static void __exit ModuleExit(void){
        printk("GoodBye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);