#include<linux/fs.h>
dev_t pdevid;
int ndevices=1;
static int __init pseudo_init(void)
{
int ret;
ret =alloc_chrdev_region(&pdevid, 0,ndevices,"pseudo_sample");
if(ret){
printk("Pesudo: Failed to register diver \n");
return -EINVAL;
}
printk("Sucessfully registered, major =%d,minor =%d\n", MAJOR(pdevid),MINOR(pdevid));
printk("Pesudo Driver Smaple..welcome\n");
return 0;
}
static void __exit psuedo_exit(void){
unregister_chrdev_region(pdevid, ndevices);
printk("Pesudo Driver Sample..Bye \n");
}
