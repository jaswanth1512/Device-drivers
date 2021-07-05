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


loff_t pcd_lseek(struct file *filp, loff_t off, int whence);
{

	return 0;
}
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
{

	return 0;
}
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
{

	return 0;
}
int pcd_open(struct inode *inode, struct file *filp);
{

	return 0;
}
int pcd_release(struct inode *inode, struct file *filp);
{

	return 0;
}

/*file operations of the driver*/
struct file_operations pcd_fops ={
	.open     = pcd_open,
	.release  = pcd_release,
	.write    = pcd_write,
	.read     = pcd_read,
	.llseek   = pcd_lseeek,
	.owner    = THIS_MODULE
};

static int __init psudo_chardrive_init(void)
{
	ret = alloc_chrdev_region(&device_number,0,1,"test name");
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
