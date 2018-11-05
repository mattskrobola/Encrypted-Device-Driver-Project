obj-m += cryptctl.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc interface.c -o interface
	sudo insmod cryptctl.ko
	sudo mknod -m 666 /dev/cryptctl c 240 0
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rm /dev/cryptctl
	sudo rmmod cryptctl
