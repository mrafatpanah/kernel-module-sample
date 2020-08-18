obj-m += char_dev.o

# buildpath = ./build

all:
	#make $(shell mkdir $(buildpath))
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean