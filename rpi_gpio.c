/*
 * \brief: Custom GPIO memory device driver for Raspberry Pi 4
 * \author: Shaun Prabhu
 * \date:   May 08, 2021
 */

// System header includes
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/uaccess.h>


// Device related macros
#define DEV_NAME_STR "RPI_GPIO"
#define MINOR_DEVSTR_FORMAT "RPI_GPIO_%d"
#define DEVMODE_FORMAT "DEVMODE=%#o"
#define DEVICE_PERMISSIONS 0666
#define MAX_MINOR_DEVS 4
#define BASE_MINOR 0
#define OK 0


// Prototypes of local functions for GPIO device
static int Rpi_Gpio_open(struct inode *inode_p, struct file *file_p);

static int Rpi_Gpio_close(struct inode *inode_p, struct file *file_p);

static long Rpi_Gpio_ioctl(struct file *file_p, unsigned int command,
                           unsigned long argument);

static ssize_t Rpi_Gpio_read(struct file *file_p, char __user *buffer_p,
                             size_t length, loff_t *offset_p);

static ssize_t Rpi_Gpio_write(struct file *file_p,
                              const char __user *buffer_p,
                              size_t length, loff_t *offset_p);

static int Rpi_Gpio_mmap(struct file *file_p,
                         struct vm_area_struct *vma_p);

static int Rpi_Gpio_uevent(struct device *dev_p,
                           struct kobj_uevent_env *env_p);


// Global variable specific to the GPIO device
static const struct file_operations rpi_gpio_fops =
{
    .owner          = THIS_MODULE,
    .open           = Rpi_Gpio_open,
    .release        = Rpi_Gpio_close,
    .unlocked_ioctl = Rpi_Gpio_ioctl,
    .read           = Rpi_Gpio_read,
    .write          = Rpi_Gpio_write,
    .mmap           = Rpi_Gpio_mmap
};

static int rpi_gpio_major_nr = 0;

static struct class *rpi_gpio_class_p = NULL; // sysfs class structure

static struct cdev rpi_gpio_cdevs[MAX_MINOR_DEVS] = {0};

// ToDo: Check if we need to use the flags commented below,
//       copied from <raspberry_pi_linux>/drivers/char/mem.c
static const struct vm_operations_struct rpi_gpio_mmap_ops = {
#ifdef CONFIG_HAVE_IOREMAP_PROT
    .access = generic_access_phys
#endif
};


// Static function implementations
static int Rpi_Gpio_open(struct inode *inode_p, struct file *file_p)
{
    printk("RPI_GPIO: Device open\n");
    return OK;
}

static int Rpi_Gpio_close(struct inode *inode_p, struct file *file_p)
{
    printk("RPI_GPIO: Device close\n");
    return OK;
}

static long Rpi_Gpio_ioctl(struct file *file_p, unsigned int command,
                           unsigned long argument)
{
    printk(KERN_INFO "RPI_GPIO: Device ioctl\n");
    return OK;
}

static ssize_t Rpi_Gpio_read(struct file *file_p, char __user *buffer_p,
                             size_t length, loff_t *offset_p)
{
    printk(KERN_INFO "RPI_GPIO: Device read\n");
    return OK;
}

static ssize_t Rpi_Gpio_write(struct file *file_p,
                              const char __user *buffer_p,
                              size_t length, loff_t *offset_p)
{
    
    
    printk(KERN_INFO "RPI_GPIO: Device write\n");
    
    return length;
}

static int Rpi_Gpio_mmap(struct file *file_p,
                         struct vm_area_struct *vma_p)
{
    int result = OK;
    size_t length = vma_p->vm_end - vma_p->vm_start;
    
    // ToDo: check all corner cases for offset as in
    //       <raspberry_pi_linux>/drivers/char/mem.c
    //
    // size_t offset = vma_p->vm_pgoff << PAGE_SHIFT;
    
    vma_p->vm_page_prot = phys_mem_access_prot(
                            file_p, vma_p->vm_pgoff, length,
                            vma_p->vm_page_prot);
    
    vma_p->vm_ops = &rpi_gpio_mmap_ops;
    
    /* Remap-pfn-range will mark the range VM_IO */
    result = remap_pfn_range(vma_p, vma_p->vm_start, vma_p->vm_pgoff,
                             length, vma_p->vm_page_prot);
    
    if(result < OK)
    {
        // ToDo: throw error here
    }
    
    printk(KERN_INFO "RPI_GPIO: Device mmap complete\n");
    
    return OK;
}

static int Rpi_Gpio_uevent(struct device *dev_p,
                           struct kobj_uevent_env *env_p)
{
    return add_uevent_var(env_p, DEVMODE_FORMAT, DEVICE_PERMISSIONS);
}

// Module specific static functions
static int __init Rpi_Gpio_init(void)
{
    int result=OK, i=0;
    dev_t gpio_dev=0, current_dev=0;
    
    // Allocate range for character device numbers for GPIO device
    result = alloc_chrdev_region(&gpio_dev, BASE_MINOR, MAX_MINOR_DEVS,
                                 DEV_NAME_STR);
    
    if(result < OK)
    {
        // ToDo: throw error here
    }
    
    // Get major number
    rpi_gpio_major_nr = MAJOR(gpio_dev);
    
    // Create device class
    rpi_gpio_class_p = class_create(THIS_MODULE, DEV_NAME_STR);
    
    if(IS_ERR(rpi_gpio_class_p))
    {
        // ToDo: throw error here
    }
    
    rpi_gpio_class_p->dev_uevent = Rpi_Gpio_uevent;
    
    // Create all required device instances
    for(i=0; ( (i < MAX_MINOR_DEVS) && (result == OK) ); i++)
    {
        current_dev = MKDEV(rpi_gpio_major_nr, i);
        
        // Initialize current device structure
        cdev_init(&rpi_gpio_cdevs[i], &rpi_gpio_fops);
        rpi_gpio_cdevs[i].owner = THIS_MODULE;
        
        // Add device to the system
        result = cdev_add(&rpi_gpio_cdevs[i], current_dev, 1);
        
        // Register device node /dev/RPI_GPIO_[i] with sysfs
        if(result == OK)
        {
            device_create(rpi_gpio_class_p, NULL, current_dev, NULL,
                          MINOR_DEVSTR_FORMAT, i);
        }
    }
    
    if(result != OK)
    {
        // ToDo: cleanup and throw error here
    }
    
    printk(KERN_INFO "RPI_GPIO: Device init successful\n");
    
    return OK; // ToDo: return result;
}

static void __exit Rpi_Gpio_exit(void)
{
    int i=0;
    dev_t current_dev=0, dev_first=0;
    
    for(i=0; i < MAX_MINOR_DEVS; i++)
    {
        current_dev = MKDEV(rpi_gpio_major_nr, i);
        
        // Delete the device
        cdev_del(&rpi_gpio_cdevs[i]);
        
        // Destroy the memory for the class created by device_create
        device_destroy(rpi_gpio_class_p, current_dev);
    }
    
    class_destroy(rpi_gpio_class_p);
    
    dev_first = MKDEV(rpi_gpio_major_nr, BASE_MINOR);
    unregister_chrdev_region(dev_first, MAX_MINOR_DEVS);
    
    printk(KERN_INFO "RPI_GPIO: Device exit successful\n");
}

// Module macros
module_init(Rpi_Gpio_init);
module_exit(Rpi_Gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shaun Prabhu");
