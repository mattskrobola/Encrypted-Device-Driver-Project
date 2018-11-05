# Encrypted-Device-Driver-Project

## Overview
- cryptctl.c acts as the control device driver
- it holds an array of structs (size is hardcoded to 20 currently) stored in kernal memory where each struct represents a encryption/decryption character driver.
- the main control driver has ioctl functions that add/remove/ect 
- right now just remove and add functionality is working.
- user app is interface.c
- code is really messy atm and super error prone so might crash vm

## How to setup
- make

- ./interface create -key-
- ./interface delete -index-

- once done testing or need to make changes make sure u run -> make clean 

- run cat /proc/devices to see if cryptctrl mapped to the right major number
- you can check if the character drivers are in /dev after creating/deleting
- You can check if that module is loaded by running lsmod
- Major number is hardcoded for the control driver to 240

## TODO
- cmd input
- ioctl calls for encrypt/decrypt change key
- Vigenère cipher
- error checks
- other
