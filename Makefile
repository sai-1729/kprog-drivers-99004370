obj-m +=step4.o
PWD=/workspace/modules/step4
KSRC=/workspace/modules/linux-4.14.202
all:
	make -C ${KSRC} M=${PWD} modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	make -C ${KSRC} M=${PWD} clean  ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
