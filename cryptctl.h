// cryptctl.h
#include <linux/ioctl.h>
// 3rd arg is what we want to get from the process to the kernel
// in the create case we want to send the key
#define IOCTL_CREATE _IOR(MAJOR_NUM, 0, char *)

#define IOCTL_DELETE _IOR(MAJOR_NUM, 1, int)

#define MAJOR_NUM 240
#define DEVICE_NAME "cryptctl"


// encryption and decryption character devices
struct crypt_dev {
	char *key;
	int open;
	struct cdev cdev;
};


