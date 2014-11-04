obj-m += simple.o
obj-m += taskPrinter.o
obj-m += taskAndChildPrinter.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
