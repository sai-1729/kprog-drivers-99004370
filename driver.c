// From the previous step to hwerer is , we dont need to use and track the read buffer and write buffer , it can be done automatically by the kfifo.
// kfifo is just circular queue , first in first out. it is struct and it has in , out, data in internal defintion.
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/kfifo.h>
#define MAX_SIZE 1024
dev_t pdevid;
typedef struct priv_obj{
	struct cdev cdev;
	struct device *pdev;
	struct kfifo kfifo;
}PRIV_OBJ;
PRIV_OBJ* pobj;
unsigned char *pbuffer;
int ndevices=1;
int rd_offset =0;
int wr_offset =0;
int buflen=0;

struct class *pclass;
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
if(kfifo_is_empty(&myfifo)){ // I guess error may be insatead pf using the myfifo they can use the kfifo. check after the code is shared.

printk("buffer is empty\n");
return 0;
}
int ret,rcount;
rcount =size;
if(rcount >kfifo_len(&myfifo)){
rcount =kfifo_len(&kfifo);
}
char *tbuf =kmalloc(rcount,GFP_KERNEL);
kfifo_out(&myfifo,tbuf,rcount);
ret =copy_to_user(ubuf,tbuf ,rcount);//check once wther pbuffer or changed contracdictaory.
if(ret)
{
printk("copy to user failed \n");
return -EFAULT;
}
kfree(tbuf);
return rcount;
}
ssize_t pseudo_write(struct file * file, const char __user * ubuf , size_t size, loff_t * off)
{
if(kfifo_is_full(&myfifo))
{
printk("buffer is full \n");

return -ENOSPC;
}

int ret,wcount;
wcount =size;
if (wcount > kfifo_avail(&myfifo))
{
wcount = fifo_avail(&myfifo)
}
char *tbuf =kmalloc(wcount,GFP_KERNEL);
ret =copy_from_user(tbuf,ubuf,wcount);//this is also changed from code to future skills code

if(ret){
printk("copy from user failed\n");
return -EFAULT;
}
kfifo_in(&myfifo,tbuf,wcount);
kfree(tbuf);//free the memory allocated to the kbuff.
//here we are copying from the user space to the kernel space using the copy from user command and crfeating another dummy
// to copy again to the kfifo.

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
pobj =kmalloc(sizeof(PRIV_OBJ),GFP_KERNEL);
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


//kfifo
kfifo_init(&kfifo,pbuffer,MAX_SIZE);

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
kfree(pobj);
kfifo_free(kfifo);
kfree(pbuffer);
}
module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SA I");
MODULE_DESCRIPTION("A SIMPLE MODULE");
