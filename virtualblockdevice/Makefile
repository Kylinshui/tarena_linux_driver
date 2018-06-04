#Makefile for driver 
PLATFORM = PC
MODNAME = virtualblockdevice
ifeq ($(PLATFORM), PC)
	KDIR ?= /lib/modules/$(shell uname -r)/build
else
	KDIR ?= /home/freescale/n_smarc_core/myandroid/kernel_imx
endif
obj-m := $(MODNAME).o
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

