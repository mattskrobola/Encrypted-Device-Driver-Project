#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include "cryptctl.h"

struct crypt_dev {
	char *key;
	int open;
	struct cdev cdev;
};
MODULE_LICENSE("GPL");

static unsigned int crypt_major = 0; // this changes
static int maxDevices = 40; // can change this
static struct crypt_dev *crypt_devices = NULL; // array to hold all devices
static struct class *crypt_class = NULL; 

//TODO 
struct file_operations encryption_fops = {

};

//dont really need these idk why they are here tbh
ssize_t  crypt_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
        printk(KERN_ALERT "inside function: %s\n", __FUNCTION__);
        return 0;
}

ssize_t crypt_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
        printk(KERN_ALERT "inside function: %s\n", __FUNCTION__);
        return 0;
}

int crypt_open(struct inode *pinode, struct file *pfile){
        printk(KERN_ALERT "inside function: %s\n", __FUNCTION__);
        return 0;
}

int crypt_close(struct inode *pinode, struct file *pfile){
        printk(KERN_ALERT "inside function: %s\n", __FUNCTION__);
        return 0;
}


static int construct_crypt_pair(struct crypt_dev *devE, struct crypt_dev *devD,  int minor, struct class *class, char *key, char *Ename, char *Dname){
	int rc = 0;

	//encryption device
	dev_t devno = MKDEV(crypt_major, minor);

	struct device *device = NULL;
	
	if(devE == NULL || class == NULL || devD == NULL){
		return -1;
	}
	devE->key = key;
	devE->open = 1;      
	cdev_init(&devE->cdev, &encryption_fops);
	devE->cdev.owner = THIS_MODULE;
	rc = cdev_add(&devE->cdev, devno, 1);
	if (rc < 0){
		return rc;
	}
	device = device_create(class, NULL, devno, NULL, Ename, minor);


	//decryption device
	minor++;
	devno = MKDEV(crypt_major, minor);
	device = NULL;
	devD->key = key;
	devD->open = 1;    
	
	cdev_init(&devD->cdev, &encryption_fops);
	devD->cdev.owner = THIS_MODULE;
	rc = cdev_add(&devD->cdev, devno, 1);
	if (rc < 0){
		return rc;
	}

	device = device_create(class, NULL, devno, NULL, Dname, minor);
	return 0;
}


static void destroy_pair(struct crypt_dev *devE, struct crypt_dev *devD, int minor, struct class *class)
{
	if(devE == NULL || class == NULL || devD == NULL){
		return;
	}
	device_destroy(class, MKDEV(crypt_major, minor));
	device_destroy(class, MKDEV(crypt_major, minor+1));

	cdev_del(&devE->cdev);
	kfree(devE->key);
	devE->open = 0;

	cdev_del(&devD->cdev);
	devD->open = 0;

	return;
}




//I started the indexes at 10 for easier string formatting
long crypt_ioctl (struct file *file, unsigned int ioctl_num, unsigned long args){
	int rc;
    char *key;
    char *tempkey;
    int index;
    int i;
    int temp;
    char nameE[15];
    char nameD[15];
    char *buffer;
    char *curKey;
    struct dataTransfer *tempDT = kzalloc(sizeof(struct dataTransfer), GFP_KERNEL);
    rc = 0;
    switch (ioctl_num) {
    case IOCTL_CREATE:
		tempkey = (char *) args;
		key = kzalloc(strnlen_user(tempkey, 50), GFP_KERNEL);
		copy_from_user(key, tempkey, strnlen_user(tempkey, 50));

		i = 0;
        while(i < maxDevices && crypt_devices[i].open != 0){
        	i += 2;
        }
        if(i == maxDevices){
        	rc = -1; // full on devices
        } else {

	        temp = i;
	        strcpy(nameE, "cryptEncrypt");
	        nameE[13] = 48 + (temp % 10);
	        temp = temp /10;
	        nameE[12] = 48 + (temp % 10);
	        nameE[14] = '\0';

	        temp = i;

	        strcpy(nameD, "cryptDecrypt");
	        nameD[13] = 48 + (temp % 10);
	        temp = temp /10;
	        nameD[12] = 48 + (temp % 10);
	        nameD[14] = '\0';

	        rc = construct_crypt_pair(&crypt_devices[i], &crypt_devices[i+1], i, crypt_class, key, &nameE[0], &nameD[0]);
    	}
        break;
    case IOCTL_DELETE:
    		index = (int) args;
    		destroy_pair(&crypt_devices[index], &crypt_devices[index+1], index, crypt_class);
    		break;
    case IOCTL_GETKEY:
    		copy_from_user(tempDT, (void *)args, sizeof(struct dataTransfer));
    		index = tempDT->index;
    		if(index >= maxDevices || crypt_devices[index].open != 1){
    			rc = -1;
    		} else {
	    		copy_to_user(tempDT->key, crypt_devices[index].key, strlen(crypt_devices[index].key));
			}
    		break;
    }
    kfree(tempDT);

	return rc;
}

//really only need crypt_ioctl here
struct file_operations  fops = {
	.owner = THIS_MODULE,
	.open = crypt_open,
	.read = crypt_read,
	.write = crypt_write,
	.unlocked_ioctl = crypt_ioctl,
	.release  = crypt_close,
};

//register device
int init_module(void) {

	dev_t dev;
	int rc;
	dev = 0;
  	rc = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);

  	if (rc < 0) {
    	printk ("failed to register cryptctl device");
    	return rc;
  	}

  	printk ("The major device number is 240.\n");
	
	rc = alloc_chrdev_region(&dev, 0, maxDevices, "testName");
	if (rc < 0) {
		printk(KERN_WARNING "[target] alloc_chrdev_region() failed\n");
		return rc;
	}
	crypt_major = MAJOR(dev);

	/* Create device class (before allocation of the array of devices) */
	crypt_class = class_create(THIS_MODULE, "cryptEncryptClass");
	if (IS_ERR(crypt_class)) {
		return -1;
	}
	crypt_devices = (struct crypt_dev *)kzalloc(
		maxDevices * sizeof(struct crypt_dev), 
		GFP_KERNEL);
  	return 0;
}

void cleanup_module(void) {
	int i;
	for(i = 0; i < maxDevices; i= i+2){
		if(crypt_devices[i].open != 0){
			destroy_pair(&crypt_devices[i], &crypt_devices[i+1], i, crypt_class);
		}
	}
	kfree(crypt_devices);
	class_destroy(crypt_class);
	unregister_chrdev_region(MKDEV(crypt_major, 0), maxDevices);
  	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}
