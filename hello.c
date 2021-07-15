#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
int __init init_module(void)
{
printk("Hello world ..welcome sai \n");
return 0;
}
void __exit cleanup_module(void)
{
printk("Bye , leaving the world\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("A Simple Module");


