#
# \brief:  Makefile to bulid custom GPIO memory device driver for Raspberry Pi 4
# \author: Shaun Prabhu
# \date:   May 08, 2021
#

BINARY      := RPI_GPIO
KERNEL      := /lib/modules/$(shell uname -r)/build
ARCH        := arm
C_FLAGS     := -Wall
KMOD_DIR    := $(shell pwd)
TARGET_PATH := /lib/modules/$(shell uname -r)/kernel/driver/char

OBJECTS := rpi_gpio.o

ccflags-y +=  $(C_FLAGS)

obj-m += $(BINARY).o

$(BINARY)-y := $(OBJECTS)

$(BINARY).ko:
	make -C $(KERNEL) M=$(KMOD_DIR) modules

install:
	cp $(BINARY).ko $(TARGET_PATH)
	depmod -a

uninstall:
	rm $(TARGET_PATH)/$(BINARY).ko
	depmod -a

clean:
	make -C $(KERNEL) M=$(KMOD_DIR) clean
