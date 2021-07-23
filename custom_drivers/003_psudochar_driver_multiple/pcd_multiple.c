#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define NO_OF_DEVICES 4
#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 1024
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 1024

char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];

struct pcdev_private_data{
	char *buffer;
	unsigned int size;
	const char *serial_number;
	int perm;
	struct cdev pcd_cdev;
};


struct pcdrv_private_data{
	dev_t device_number;
	int total_devices;
	struct class *class_pcd;
	struct device *device_pcd;
	struct pcdev_private_data pcdevData[NO_OF_DEVICES];
};

struct pcdrv_private_data pcdrvData = {
	.total_devices = NO_OF_DEVICES,
	.pcdevData = {
		[0] = {
			.buffer = device_buffer_pcdev1,
			.size = MEM_SIZE_MAX_PCDEV1,
			.serial_number = "PCDEV1XYZ123",
			.perm = 0x1/*RDONLY*/
		},
		[1] = {
			.buffer = device_buffer_pcdev2,
			.size = MEM_SIZE_MAX_PCDEV2,
			.serial_number = "PCDEV2XYZ123",
			.perm = 0x10/*WRONLY*/
		},
		[2] = {
			.buffer = device_buffer_pcdev3,
			.size = MEM_SIZE_MAX_PCDEV3,
			.serial_number = "PCDEV3XYZ123",
			.perm = 0x11/*RDWR*/
		},
		[3] = {
			.buffer = device_buffer_pcdev4,
			.size = MEM_SIZE_MAX_PCDEV4,
			.serial_number = "PCDEV4XYZ123",
			.perm = 0x1/*RDWR*/
		}
	}
};
int check_permission(void)
{
	return 0;
}
loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
#if 0
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
#endif
return 0;
}
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)filp->private_data;
	int max_size = pcdev_data->size;
	printk("read is called for %zu bytes\n",count);
	printk("value of pile_position is %lld",*f_pos);
	
	if(*f_pos + count > max_size)
	{
		count = max_size - (*f_pos);
	}
	/*copy to user */
	if(copy_to_user(buff ,pcdev_data->buffer + (*f_pos),count) != 0)
	{
		return -EFAULT;
	}
	/* update current file position*/
	*f_pos += count;
	printk("No of bytes succesfully read = %zu\nupdate file position = %lld",count,*f_pos);
	return count;

return 0;
}
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)filp->private_data;
	int max_size = pcdev_data->size;
	printk("Write is called for %zu bytes\n",count);
        printk("value of file_position is %lld",*f_pos);
	if(count + (*f_pos) > max_size)
	{
		count = max_size - (*f_pos);
	}
	if(!count)
		return -ENOMEM;
	if(copy_from_user(pcdev_data->buffer + (*f_pos), buff, count) != 0)
	{
		return -EFAULT;
	}
	*f_pos += count;
        printk("No of bytes succesfully read = %zu\nupdate file position = %lld",count,*f_pos);
	return count;

return 0;
}
int pcd_open(struct inode *inode, struct file *filp)
{
	int ret;
	int minor_num;
	struct pcdev_private_data *pdev_data;
	minor_num = MINOR(inode->i_rdev);
	pdev_data = container_of(inode->i_cdev,struct pcdev_private_data, pcd_cdev);
	filp->private_data = pdev_data;
	ret = check_permission();
	if(ret != 0)
	{
		printk("open is succcesfull\n");
	}
	else
	{
		printk("open is not succcesfull\n");
	}
return 0;
}
int pcd_release(struct inode *inode, struct file *filp)
{
#if 0
	printk("release is called");
	return 0;
#endif
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

static int __init psudo_chardrive_init(void)
{

	int ret;
	int i = 0;
	ret = alloc_chrdev_region(&pcdrvData.device_number,0,NO_OF_DEVICES,"pcd_devices");
	if(ret < 0)
		goto OUT;
		
	pcdrvData.class_pcd = class_create(THIS_MODULE,"PCD_CLASS");
	if(IS_ERR(pcdrvData.class_pcd))
	{
		printk("class creation failed\n");
		ret = PTR_ERR(pcdrvData.class_pcd);
		goto UNREG_CHRDEV;
	}	
	for(i = 0 ; i < NO_OF_DEVICES; i++ )
	{
		printk("Major and Minor numbers are =  %d :%d \n",MAJOR(pcdrvData.device_number + i),MINOR(pcdrvData.device_number + i));
		cdev_init(&pcdrvData.pcdevData[i].pcd_cdev,&pcd_fops);
		pcdrvData.pcdevData[i].pcd_cdev.owner = THIS_MODULE;
		ret = cdev_add(&pcdrvData.pcdevData[i].pcd_cdev,pcdrvData.device_number + i ,1);
		if(ret < 0)
        	        goto CDEV_DEL;
		pcdrvData.device_pcd = device_create(pcdrvData.class_pcd,NULL,pcdrvData.device_number + i,NULL,"pcdev-%d",i);
		if(IS_ERR(pcdrvData.device_pcd))
		{
			printk("device create failed\n");
			ret = PTR_ERR(pcdrvData.device_pcd);
			goto CLASS_DEL;
		}
		printk("init is succesfull\n");
		return 0;
	}
CDEV_DEL:
CLASS_DEL:
	for(; i >0;i--)
	{
		device_destroy(pcdrvData.class_pcd,pcdrvData.device_number + i);
		cdev_del(&pcdrvData.pcdevData[i].pcd_cdev);
	}
	class_destroy(pcdrvData.class_pcd);
UNREG_CHRDEV:
	unregister_chrdev_region(pcdrvData.device_number, NO_OF_DEVICES);
OUT:
	return ret;
}


static void __exit psudo_chardrive_exit(void)
{
	int i = 0;
	for(i = 0; i < NO_OF_DEVICES ; i++)
	{
		device_destroy(pcdrvData.class_pcd,pcdrvData.device_number + i);
		cdev_del(&pcdrvData.pcdevData[i].pcd_cdev);
	}
	class_destroy(pcdrvData.class_pcd);
	unregister_chrdev_region(pcdrvData.device_number, NO_OF_DEVICES);
	printk("Unloaded module");
}

module_init(psudo_chardrive_init);
module_exit(psudo_chardrive_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JASWANTH");
MODULE_DESCRIPTION("PSUDO CHAR DERIVR WITH MULTIPLE DEVICES");
