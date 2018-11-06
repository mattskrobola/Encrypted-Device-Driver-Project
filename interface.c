#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cryptctl.h"

/*
  Makes ioctl call to the "control device" to create an encryption decryption pair
*/
int ioctl_create(int fd, char *key) {
    int rc;
    rc = ioctl(fd, IOCTL_CREATE, key);
    if (rc < 0) {
        printf ("ioctl_create failed\n");
        return -1;
    }

    printf("created cryptEncrypt%d and cryptDecrypt%d\n", rc, rc);
}

/*
  Makes ioctl call to the "control device" to remove the pair given an index
*/
int ioctl_delete(int fd, int index){
    int rc;
    rc = ioctl(fd, IOCTL_DELETE, index);

    if (rc < 0) {
        printf ("ioctl_delete failed");
        return -1;
    }
    printf("removed cryptEncrypt%d and cryptDecrypt%d\n", index, index);
}

/*
  
*/
int ioctl_getkey(int fd, int index, char *key) {
    struct dataTransfer dt = {key , index};
    long rc;
    rc = ioctl(fd, IOCTL_GETKEY, &dt);
    if (rc < 0) {
        printf ("ioctl_getkey failed\n");
        return rc;
    }
    return 0;
    
}

//TODO cmd input
int main(int arg, char* argv[]){

    int fd;
    int rc;
    char *key = "hey\n";
    int index;

    
    if(arg < 2){
        printf("Didn't pass any arguments \n");
        return -1;
    }
    fd = open("/dev/cryptctl", 0);
    if (fd < 0) {
        printf("Can't open device file\n");
        return -1;
    }
    if(strcmp(argv[1], "create") == 0){
        char *key = argv[2];
        ioctl_create(fd,key);
    } else if (strcmp(argv[1], "delete") == 0){
        index = atoi(argv[2]);
        ioctl_delete(fd,index); 
    } else if(strcmp(argv[1], "encrypt") == 0){
        index = atoi(argv[2]);
        char getkey[50];
        ioctl_getkey(fd, index, &getkey[0]);
        printf("key: %s", getkey);
    }

    close(fd); 
    return 0;  
}
