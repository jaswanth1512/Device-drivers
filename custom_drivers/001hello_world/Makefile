obj-m :=main.o

KDIR = /lib/modules/5.11.0-22-generic/build/
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
help:
	make -C $(KDIR) M=$(PWD) help
