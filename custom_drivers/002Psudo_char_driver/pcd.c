#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

/* defining devixce number*/
#define DEV_MEM 512
char device_buffer[DEV_MEM];

/* this holds the device number*/
dev_t device_number;

/*creating cdev variable*/
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{

	loff_t temp;
	printk("Lssek function is called\n");
	switch(whence)
	{
		case SEEK_SET:
			if((offset > DEV_MEM) || (offset < 0))
				return -EINVAL;
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > DEV_MEM) || (temp < 0))
				return -EINVAL;
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = DEV_MEM +  offset;
			if((temp > DEV_MEM) || (temp < 0))
                                return -EINVAL;
			filp->f_pos = DEV_MEM + offset;
			break;
		default:
			return -EINVAL;
	}
	return filp->f_pos;
}
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	printk("read is called for %zu bytes\n",count);
	printk("value of pile_position is %lld",*f_pos);
	/*1) check count */
	if(*f_pos + count > DEV_MEM)
	{
		count = DEV_MEM - (*f_pos);
	}
	/*copy to user */
	if(copy_to_user(buff ,&device_buffer[*f_pos],count) != 0)
	{
		return -EFAULT;
	}
	/* update current file position*/
	*f_pos += count;
	printk("No of bytes succesfully read = %zu\nupdate file position = %lld",count,*f_pos);
	return count;
}
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	printk("Write is called for %zu bytes\n",count);
        printk("value of file_position is %lld",*f_pos);
	if(count + (*f_pos) > DEV_MEM)
	{
		count = DEV_MEM - (*f_pos);
	}
	if(!count)
		return -ENOMEM;
	if(copy_from_user(&device_buffer[*f_pos], buff, count) != 0)
	{
		return -EFAULT;
	}
	*f_pos += count;
        printk("No of bytes succesfully read = %zu\nupdate file position = %lld",count,*f_pos);
	return count;
}
int pcd_open(struct inode *inode, struct file *filp)
{
	printk("open is called\n");
	return 0;
}
int pcd_release(struct inode *inode, struct file *filp)
{
	
	printk("release is called");
	return 0;
}

/*file operations of the driver*/
struct file_operations pcd_fops ={
	.open     = pcd_open,
	.release  = pcd_release,
	.write    = pcd_write,
	.read     = pcd_read,
	.llseek   = pcd_lseek,
	.owner    = THIS_MODULE
};

struct class *class_pcd;
struct device *device_pcd;

static int __init psudo_chardrive_init(void)
{
	alloc_chrdev_region(&device_number,0,1,"pcd_devices");
	printk("Major and Minor numbers are =  %d :%d \n",MAJOR(device_number),MINOR(device_number));
	cdev_init(&pcd_cdev,&pcd_fops);
//	pcd_cdev.owner = THIS_MODULE;
	cdev_add(&pcd_cdev,device_number,1);
	class_pcd = class_create(THIS_MODULE,"PCD_CLASS");
	device_pcd = device_create(class_pcd,NULL,device_number,NULL,"PCD");
	printk("init is succesfull\n");
	return 0;
}


static void __exit psudo_chardrive_exit(void)
{
	device_destroy(class_pcd, device_number);
	class_destroy(class_pcd);
	cdev_del(&pcd_cdev);
	unregister_chrdev_region(device_number,1);
	printk("Device unregistered\n");
}

module_init(psudo_chardrive_init);
module_exit(psudo_chardrive_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JASWANTH");
MODULE_DESCRIPTION("PSUDO CHAR DERIVR");
