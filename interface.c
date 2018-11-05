//#include "cryptctl.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/ioctl.h>


// 3rd arg is what we want to get from the process to the kernel
// in the create case we want to send the key
#define IOCTL_CREATE _IOR(MAJOR_NUM, 0, char *)

#define IOCTL_DELETE _IOR(MAJOR_NUM, 1, int)

#define MAJOR_NUM 240
#define DEVICE_NAME "cryptctl"

int ioctl_create(int fd, char *key)
{
  int rc;
  
  rc = ioctl(fd, IOCTL_CREATE, key);

  if (rc < 0) {
    printf ("ioctl_create failed");
    return -1;
  }

  printf("created character driver");
}

int ioctl_delete(int fd, int index)
{
  int rc;

  rc = ioctl(fd, IOCTL_DELETE, index);

  if (rc < 0) {
    printf ("ioctl_create failed");
    return -1;
  }

  printf("created cryptEncrypt:%d\n", index);
}


//TODO cmd input
int main(int arg, char* argv[])
{

	int fd;
  	int rc;
  	char *key = "hey\n";
  	int index;
  	 fd = open("/dev/cryptctl", 0);
  	if (fd < 0) {
    	printf("Can't open device file");
    	return -1;
  	}
  	
	if(arg < 2){
		printf("Didn't pass any arguments \n");
		return -1;
	}
	if(strcmp(argv[1], "create") == 0){
		char *key = argv[2];
		ioctl_create(fd,key);
	} else if (strcmp(argv[1], "delete") == 0){
		index = atoi(argv[2]);
		ioctl_delete(fd,index);	
	}

  close(fd); 
  return 0;  
}
