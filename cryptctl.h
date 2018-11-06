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

//encrypts input given a key and input. Returns a char array
char* encryptString(char* key, char* input){
  int messageLength = strlen(input);
	int keyLen = strlen(key);
	char *encryptedMsg= (char *) malloc(sizeof(char) * messageLength);

	int i = 0, j = 0;
	for(; i < messageLength; ++i, ++j){
    if(j == keyLen){
          j = 0;
    }
    char currKey;
    if ((key[i] >= 'A') && (key[i]<=  'Z')){
           currKey = (key[i] -  'A');
    }
    if ((key[i] >= 'a') && (key[i] <= 'z')){
           currKey = (key[i] - 'a');
    }
    int currLetter = input[i] + currKey;
    if (isupper(input[i]) && (currLetter > 'Z')){
      currLetter = (currLetter - 26);
    }
    if(islower(input[i]) && (currLetter > 'z')){
      currLetter = (currLetter - 26);
    }



    encryptedMsg[i] = currLetter;

 	}
  encryptedMsg[messageLength] = '\0';
	return encryptedMsg;
}

//decrypts given encrypted input given a key. Returns char array
char* decryptString(char* key, char* encryptedInput){
  int messageLength = strlen(encryptedInput);
	int keyLen = strlen(key);

	char *decryptedMsg= (char *) malloc(sizeof(char) * messageLength);

	int i = 0, j = 0;
	for(; i < messageLength; ++i, ++j){
    if(j == keyLen){
          j = 0;
    }
    char currKey;
    if ((key[i] >= 'A') && (key[i]<=  'Z')){
           currKey = (key[i] -  'A');
    }
    if ((key[i] >= 'a') && (key[i] <= 'z')){
           currKey = (key[i] - 'a');
    }

    int currLetter = encryptedInput[i] - currKey + 26;
    if (isupper(encryptedInput[i]) && (currLetter > 'Z')){
      currLetter = (currLetter - 26);
    }
    //printf("%d\n", currLetter);
    if(islower(encryptedInput[i]) && (currLetter > 'z')){
      currLetter = (currLetter - 26);
    }
    decryptedMsg[i] = currLetter;

 	}
  decryptedMsg[messageLength] = '\0';
	return decryptedMsg;
}
