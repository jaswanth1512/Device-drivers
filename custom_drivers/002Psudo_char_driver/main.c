#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

/* defining devixce number*/
#define DEV_MEM 512
char device_buffer[DEV_MEM];

/* this holds the device number*/
dev_t device_number;

/*creating cdev variable*/
struct cdev pcd_cdev;

/*file operations of the driver*/
struct file_operations pcd_fops;

static int __init psudo_chardrive_init(void)
{
	alloc_chrdev_region(&device_number,0,1,"test name");
	cdev_init(&pcd_cdev,&pcd_fops);
	pcd_cdev.owner = THIS_MODULE;
	cdev_add(&pcd_cdev,device_number,1);
	return 0;
}

static void __exit psudo_chardrive_exit(void)
{

}

module_init(psudo_chardrive_init);
module_exit(psudo_chardrive_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JASWANTH");
MODULE_DESCRIPTION("PSUDO CHAR DERIVR");
