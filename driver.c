
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#define MAX_SIZE 1024

struct cdev cdev;
dev_t pdevid;
int ndevices=1;
struct device *pdev;
struct class *pclass;

unsigned char *pbuffer;
int rd_offset =0;
int wr_offset =0;
int buflen=0;


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
ssize_t pseudo_read(struct file * file, char __user * ubuf , size_t size, loff_t * off)
{
printk("Pseudo--read method \n");
if(buflen==0){

printk("buffer is empty\n");
return 0;
}
int ret,rcount;
rcount =size;
if(rcount >buflen)
rcount =buflen;
ret =copy_to_user(ubuf,pbuffer ,rcount);//check once wther pbuffer or changed contracdictaory.
if(ret)
{
printk("copy to user failed \n");
return -EFAULT;
}
rd_offset +=rcount;
buflen-=rcount;
return rcount;
}
ssize_t pseudo_write(struct file * file, const char __user * ubuf , size_t size, loff_t * off)
{
if(wr_offset>=MAX_SIZE)
{
printk("buffe is full \n");

return -ENOSPC;
}

int ret,wcount;
wcount =size;
if (wcount > MAX_SIZE -wr_offset)
{
wcount = MAX_SIZE -wr_offset;
}
ret =copy_from_user(pbuffer,ubuf,wcount);//this is also changed from code to future skills code

if(ret){
printk("copy from user failed\n");
return -EFAULT;
}
wr_offset +=wcount;
buflen+=wcount;

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
int i=0;
pbuffer =kmalloc(MAX_SIZE,GFP_KERNEL);
pclass =class_create(THIS_MODULE,"pseudo_class");
ret =alloc_chrdev_region(&pdevid, 0,ndevices,"pseudo_sample");
if(ret){
printk("Pesudo: Failed to register diver \n");
return -EINVAL;
}
cdev_init(&cdev,&fops);
kobject_set_name(&cdev.kobj,"pdevice%d",1);
ret =cdev_add(&cdev,pdevid,1);
pdev =device_create(pclass,NULL,pdevid,NULL,"psample%d",i);
printk("Sucessfully registered, major =%d,minor =%d\n", MAJOR(pdevid),MINOR(pdevid));
printk("Pesudo Driver Smaple..welcome\n");
return 0;
}
static void __exit psuedo_exit(void){
cdev_del(&cdev);
device_destroy(pclass,pdevid);
unregister_chrdev_region(pdevid, ndevices);
printk("Pesudo  Driver Sample..Bye \n");
class_destroy(pclass);
kfree(pbuffer);
}
module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SA I");
MODULE_DESCRIPTION("A SIMPLE MODULE");
