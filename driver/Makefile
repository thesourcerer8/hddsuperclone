
# Suppress module signature verification: Get rid of kernel messages like
# >> module verification failed: signature and/or required key missing -
# >> tainting kernel
CONFIG_MODULE_SIG=n

KMOD = hddsuperclone_driver
TMPS =  $(KMOD).mod.*  \
       .$(KMOD).*.cmd  \
        built-in.o     \
       .built-in.*     \
        modules.order  \
        Module.symvers \
       .tmp_versions

ifneq ($(KERNELRELEASE),)
    obj-m = $(KMOD).o
else
    MAKE_FLAGS = -C
    KDIR      ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) $(MAKE_FLAGS)$(KDIR) M=$$PWD
endif

# Specify flags and other vars here.
RMFLAGS = -vR

.PHONY: all clean

all: default

clean:
	$(RM) $(RMFLAGS) $(KMOD).ko $(KMOD).o $(TMPS)

install:
	insmod $(KMOD).ko

uninstall:
	rmmod $(KMOD)

# vim:set nu:et:ts=4:sw=4:
