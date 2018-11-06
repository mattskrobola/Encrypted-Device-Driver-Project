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
    encryptedMsg[i] = ((input[i] + key[i]) % 26) + 'A';
    // printf("input letter = %c, digit = %d\n", input[i], input[i]);
    // printf("key letter = %c, digit = %d\n", key[i], key[i]);
    // printf("encryptedMsg letter = %c, digit = %d\n", encryptedMsg[i], encryptedMsg[i]);
    // printf("current digit for j = %d\n", j);
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
    decryptedMsg[i] = (((encryptedInput[i] - key[i]) + 26) % 26) + 'A';

 	}
  decryptedMsg[messageLength] = '\0';
	return decryptedMsg;
}
