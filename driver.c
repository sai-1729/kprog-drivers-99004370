
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/cdev.h>
struct cdev cdev;
dev_t pdevid;
int ndevices=1;


int pseudo_open(struct inode* inode , struct file* file)
{
printk("Pseudo--open method \n");
return 0;
}

int pseudo_close(struct inode* inode , struct file* file)
{
printk("Pseudo--release method \n");
return 0;
}
ssize_t pseudo_read(struct file * file, char __user * buf , size_t size, loff_t * off)
{
printk("Pseudo--read method \n");
return 0;
}
ssize_t pseudo_write(struct file * file, const char __user * buf , size_t size, loff_t * off)
{
printk("Pseudo--write method \n");
return -ENOSPC;
}
struct file_operations fops ={
	.open =pseudo_open,
	.release =pseudo_close,
	.write =pseudo_write,
	.read = pseudo_read
};




static int __init psuedo_init(void)
{
int ret;
ret =alloc_chrdev_region(&pdevid, 0,ndevices,"pseudo_sample");
if(ret){
printk("Pesudo: Failed to register diver \n");
return -EINVAL;
}
cdev_init(&cdev,&fops);
kobject_set_name(&cdev.kobj,"pdevice%d",1);
ret =cdev_add(&cdev,pdevid,1);

printk("Sucessfully registered, major =%d,minor =%d\n", MAJOR(pdevid),MINOR(pdevid));
printk("Pesudo Driver Smaple..welcome\n");
return 0;
}
static void __exit psuedo_exit(void){
unregister_chrdev_region(pdevid, ndevices);
cdev_del(&cdev);
printk("Pesudo  Driver Sample..Bye \n");
}
module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI");
MODULE_DESCRIPTION("A SIMPLE MODULE");
