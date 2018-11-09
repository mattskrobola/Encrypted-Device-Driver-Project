#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cryptctl.h"

/*
  Makes ioctl call to the "control device" to create an encryption decryption pair
*/
int ioctl_create(int fd, char *key) {
    int rc;
    rc = ioctl(fd, IOCTL_CREATE, key);
    if (rc < 0) {
        printf ("Not enough space to create device pair\n");
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
        printf ("index %d doesn't exist \n", index);
        return -1;
    }
    printf("removed cryptEncrypt%d and cryptDecrypt%d\n", index, index);
}

/*
  
*/
int ioctl_getkey(int fd, int index, char *key) {
    struct dataTransfer dt = {key , index};
    int rc;
    rc = ioctl(fd, IOCTL_GETKEY, &dt);
    if (rc < 0) {
        printf ("Index %d does not exist\n", index);
        return rc;
    }
    printf("current key for pair %d is %s\n", index, dt.key);
    return 0;
    
}

int ioctl_changekey(int fd, int index, char *key) {
    struct dataTransfer dt = {key , index};
    int rc;
    rc = ioctl(fd, IOCTL_CHANGEKEY, &dt);
    if (rc < 0) {
        printf ("index %d doesn't exist\n", index);
        return rc;
    }
    printf("key for cryptEncrypt%d and cryptDecrypt%d changed to: %s\n", index, index, key);
    return 0;
    
}

// encryption and decryption character devices

//encrypts input given a key and input.
void encryptString(char *key, char *input, char *buffer){
    int messageLength, keyLen, j, i, currLetter;
    char currKey;
    messageLength = strlen(input);
    keyLen = strlen(key);
    i = 0;
    j = 0;
	
    for(; i < messageLength; ++i, ++j){
    	if(input[i] <  65 || input[i] > 122 || input[i] > 90 && input[i] < 97){
			currLetter = input[i];
            j--;
		}
		else{
    		if(j == keyLen){
      	    j = 0;
    		}
    		if ((key[j] >= 'A') && (key[j]<=  'Z')){
           	currKey = (key[j] -  'A');
    		}
    		if ((key[j] >= 'a') && (key[j] <= 'z')){
           	currKey = (key[j] - 'a');
    		}
    		
    		currLetter = input[i] + currKey;
    		if (isupper(input[i]) && (currLetter > 'Z')){
      		currLetter = (currLetter - 26);
    		}
    		if(islower(input[i]) && (currLetter > 'z')){
      		currLetter = (currLetter - 26);
    		}
		}
    	buffer[i] = currLetter;

    }
    buffer[messageLength] = '\0';
    return;
}

//decrypts given encrypted input given a key.
void decryptString(char *key, char *encryptedInput, char *buffer){
    int messageLength, keyLen, j, i, currLetter;
    char currKey;
    messageLength = strlen(encryptedInput);
    keyLen = strlen(key);
    i = 0;
    j = 0;
    for(; i < messageLength; ++i, ++j){
    	if(encryptedInput[i] <  65 || encryptedInput[i] > 122 || encryptedInput[i] > 90 && encryptedInput[i] < 97){
			currLetter = encryptedInput[i];
            j--;
		}
		else{
    		if(j == keyLen){
                j = 0;
    		}
    		if ((key[j] >= 'A') && (key[j]<=  'Z')){
           	    currKey = (key[j] -  'A');
    		}
    		if ((key[j] >= 'a') && (key[j] <= 'z')){
         	    currKey = (key[j] - 'a');
    		}
    		currLetter = encryptedInput[i] - currKey + 26;
    		if (isupper(encryptedInput[i]) && (currLetter > 'Z')){
      	        currLetter = (currLetter - 26);
    		}
    		//printf("%d\n", currLetter);
    		if(islower(encryptedInput[i]) && (currLetter > 'z')){
      		    currLetter = (currLetter - 26);
    		}
    	}
    		buffer[i] = currLetter;

    }
    buffer[messageLength] = '\0';
    return;
}

//TODO cmd input
int main(int arg, char* argv[]){
    int i;
    int j;
    int fd;
    int rc;
    int cur;
    char *curS;
    char *key;
    char input[100];
    int index;
    char getkey[50]; // holds the fetched key
    char buffer[200]; // holds the result can change size if needed

    
    if(arg < 2){
        printf("Didn't pass any arguments \n");
        return -1;
    }
    fd = open("/dev/cryptctl", 0);
    if (fd < 0) {
        printf("Can't open device file\n");
        return -1;
    }
    if(strcmp(argv[1], "create") == 0 && arg == 3){
        key = argv[2];
        ioctl_create(fd,key);
    } else if (strcmp(argv[1], "delete") == 0 && arg == 3){
        index = atoi(argv[2]);
        ioctl_delete(fd,index); 
    } else if(strcmp(argv[1], "encrypt") == 0 && arg >= 4){
        index = atoi(argv[2]);
        rc = ioctl_getkey(fd, index, &getkey[0]);
        cur = 0;
        strcpy(input, "");
        if(rc > -1){
            for(i = 3; i < arg; i++){
                cur += strlen(argv[i]) + 1;
                strcat(input, argv[i]);
                strcat(input," ");
            }
            cur--;
            input[cur] = '\0';
            encryptString(&getkey[0], &input[0], &buffer[0]);
            printf("encryped result: %s\n", buffer);
        }

    } else if(strcmp(argv[1], "change_key") == 0 && arg == 4){
        key = argv[2];
        key[strlen(key)] = '\0';
        index = atoi(argv[3]);
        ioctl_changekey(fd, index, key);
    } else if(strcmp(argv[1], "decrypt") == 0 && arg >= 4){
        index = atoi(argv[2]);
        rc = ioctl_getkey(fd, index, &getkey[0]);
        cur = 0;
        strcpy(input, "");
        if(rc > -1){
            for(i = 3; i < arg; i++){
                cur += strlen(argv[i]) + 1;
                strcat(input, argv[i]);
                strcat(input," ");
            }
            cur--;
            input[cur] = '\0';
            decryptString(&getkey[0], &input[0], &buffer[0]);
            printf("decryped result: %s\n", buffer);
        }
    } else if(strcmp(argv[1], "getkey") == 0 && arg == 3){
        index = atoi(argv[2]);
        ioctl_getkey(fd, index, &getkey[0]);
    } else {
	printf("wrong combination of arugments\n");
    }

    close(fd); 
    return 0;  
}

