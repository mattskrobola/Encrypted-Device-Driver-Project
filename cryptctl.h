#include <linux/ioctl.h>

#define MAJOR_NUM 240
#define DEVICE_NAME "cryptctl"

struct dataTransfer {
	char *key;
	int index;
};
// 3rd arg is what we want to get from the process to the kernel
// in the create case we want to send the key
#define IOCTL_CREATE _IOR(MAJOR_NUM, 0, char *)
#define IOCTL_DELETE _IOR(MAJOR_NUM, 1, int)
#define IOCTL_GETKEY _IOR(MAJOR_NUM, 2, void *)

// encryption and decryption character devices


