# Encrypted-Device-Driver-Project

## Overview
- cryptctl.c acts as the control device driver
- it holds an array of structs stored in kernal memory where each struct represents a encryption/decryption character driver.
- the main control driver has ioctl functions that add/remove/ect
- user app is interface.c

## How to setup
- make

- ./interface create -key
- ./interface delete -index
- ./interface encrypt -index -input
- ./interface decrypt -index -input
- ./interface change_key -newkey -index
- ./interface getkey -index

- once done testing or need to make changes make sure u run -> make clean 

- run cat /proc/devices to see if cryptctrl mapped to the right major number
- you can check if the character drivers are in /dev after creating/deleting
- You can check if that module is loaded by running lsmod
- Major number is hardcoded for the control driver to 240

## TODO
- test
- allow for multiple words as input for encrypt decrpt
- report
- maybe extra credit
