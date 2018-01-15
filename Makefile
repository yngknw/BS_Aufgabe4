obj-m:=tzm.o #goal-definition, module that should be build is tzm.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules #switches to kernel directory
	#before performing make, M=$(PWD)tells make where the project files exist
	#modules the default target for kernel modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
	rm -f modules.order Module.symvers