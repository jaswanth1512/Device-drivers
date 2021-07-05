obj-m := main.o

KDIR = /usr/lib/modules/5.11.0-22-generic/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
