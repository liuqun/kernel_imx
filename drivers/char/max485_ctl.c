#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

//#define GPS_DEBUG
#ifdef GPS_DEBUG
#define DPRINTK(x...) printk("MAX485_CTL DEBUG:" x)
#else
#define DPRINTK(x...)
#endif

#define DEVICE_NAME				"max485_ctl"
#define DRIVER_NAME "max485_ctl"

#define MAX485_GPIO			IMX_GPIO_NR(3, 1)
int max485_ctl_open(struct inode *inode,struct file *filp)
{
	DPRINTK("Device Opened Success!\n");
	return nonseekable_open(inode,filp);
}

int max485_ctl_release(struct inode *inode,struct file *filp)
{
	DPRINTK("Device Closed Success!\n");
	return 0;
}

int max485_ctl_pm(bool enable)
{
	int ret = 0;
	printk("firecxx debug: GPS PM return %d\r\n" , ret);
	return ret;
};

long max485_ctl_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	printk("firecxx debug: max485_ctl_ioctl cmd is %d\n" , cmd);

	switch(cmd)
	{		
		case 1:
			if(gpio_request(MAX485_GPIO ,"DEVICE_NAME"))
			{
				DPRINTK("max485_ctl GPIO err!\r\n");
			}
			else
			{
				gpio_direction_output(MAX485_GPIO, 1);
				DPRINTK("max485_ctl Set High!\n");
				gpio_free(MAX485_GPIO);

				mdelay(100);
			}
				
			break;
		case 0:
			if(gpio_request(MAX485_GPIO ,"DEVICE_NAME"))
			{
				DPRINTK("max485_ctl GPIO err!\r\n");
			}
			else
			{			
				gpio_direction_output(MAX485_GPIO,0);
				DPRINTK("max485_ctl Set Low!\n");
				gpio_free(MAX485_GPIO);

				mdelay(100); 
			}
			
			break;
			
		default:
			DPRINTK("max485_ctl COMMAND ERROR!\n");
			return -ENOTTY;
	}
	return 0;
}

static struct file_operations max485_ctl_ops = {
	.owner 	= THIS_MODULE,
	.open 	= max485_ctl_open,
	.release= max485_ctl_release,
	.unlocked_ioctl 	= max485_ctl_ioctl,
};

static struct miscdevice max485_ctl_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &max485_ctl_ops,
	.name	= "max485_ctl_pin",
};


static int max485_ctl_probe(struct platform_device *pdev)
{
	int err = 0;
	
	int ret;
	char *banner = "max485_ctl Initialize\n";

	printk(banner);
	gpio_free(MAX485_GPIO);
	err = gpio_request(MAX485_GPIO, "DEVICE_NAME");
	if (err) {
		printk(KERN_ERR "failed to request DEVICE_NAME for "
			"max485_ctl control\n");
		return err;
	}
	gpio_direction_output(MAX485_GPIO, 1);

		gpio_direction_output(MAX485_GPIO, 0);
		gpio_free(MAX485_GPIO);
     
	  
	ret = misc_register(&max485_ctl_dev);
	if(ret<0)
	{
		printk("max485_ctl:register device failed!\n");
		goto exit;
	}

	return 0;

exit:
	misc_deregister(&max485_ctl_dev);
	return ret;
}

static int max485_ctl_remove (struct platform_device *pdev)
{
	misc_deregister(&max485_ctl_dev);	

	return 0;
}

static int max485_ctl_suspend (struct platform_device *pdev, pm_message_t state)
{
	DPRINTK("max485_ctl suspend:power off!\n");
	return 0;
}

static int max485_ctl_resume (struct platform_device *pdev)
{
	DPRINTK("max485_ctl resume:power on!\n");
	return 0;
}

static struct platform_driver max485_ctl_driver = {
	.probe = max485_ctl_probe,
	.remove = max485_ctl_remove,
	.suspend = max485_ctl_suspend,
	.resume = max485_ctl_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static void __exit max485_ctl_exit(void)
{
	platform_driver_unregister(&max485_ctl_driver);
}

static int __init max485_ctl_init(void)
{	
	return platform_driver_register(&max485_ctl_driver);
}

module_init(max485_ctl_init);
module_exit(max485_ctl_exit);

MODULE_LICENSE("Dual BSD/GPL");
