#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cryptctl.h"

/*
  get the key
*/
int ioctl_getkey(int fd, int index, char *key) {
    struct dataTransfer dt = {key , index};
    int rc;
    rc = ioctl(fd, IOCTL_GETKEY, &dt);
    if (rc < 0) {
        printf ("Index does not exist\n");
        return rc;
    }
    printf("current key for pair %d is %s\n", index, dt.key);
    return 0;
    
}
/*
  change the key
*/
int ioctl_changekey(int fd, int index, char *key) {
    struct dataTransfer dt = {key , index};
    int rc;
    rc = ioctl(fd, IOCTL_CHANGEKEY, &dt);
    if (rc < 0) {
        printf ("index doesn't exist\n");
        return rc;
    }
    printf("key for cryptEncrypt%d and cryptDecrypt%d changed to: %s\n", index, index, key);
    return 0;
    
}

int main(int arg, char* argv[]){

    int fd;
    int rc;
    char *key;
    char *input;
    int index;
    char getkey[50]; // holds the fetched key
    char buffer[200]; // holds the result can change size if needed

    if(arg < 2){
        printf("Didn't pass any arguments \n");
        return -1;
    }
    fd = open("/sys/cryptctl", 0);
    if (fd < 0) {
        printf("Can't open device file\n");
        return -1;
    } else if(strcmp(argv[1], "change_key") == 0 && arg == 4){
        key = argv[2];
        key[strlen(key)] = '\0';
        index = atoi(argv[3]);
        ioctl_changekey(fd, index, key);
    } else if(strcmp(argv[1], "getkey") == 0 && arg == 3){
        index = atoi(argv[2]);
        ioctl_getkey(fd, index, &getkey[0]);
    } else {
	printf("wrong combination of arugments\n");
    }

    close(fd); 
    return 0;  
}
