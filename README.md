# Encrypted-Device-Driver-Project

## Overview
- cryptctl.c acts as the control device driver
- it holds an array of structs (size is hardcoded to 20 currently) stored in kernal memory where each struct represents a encryption/decryption character driver.
- the main control driver has ioctl functions that add/remove/ect 
- right now just remove and add functionality is working.
- code is really messy atm and super error prone so might crash vm

## How to setup
- make
- sudo insmod cryptctl.ko (this loads the module)
- sudo mknod -m 666 /dev/cryptctl c 240 0 (puts cryptctl in /dev we can put this in the makefile later)
- gcc interface.c -o interface

- ./interface create -key-
- ./interface delete -index-

- once done testing or need to make changes make sure u run 
  - rm /dev/cryptctl
  - sudo rmmod cryptctl
  - then repeat everything

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
