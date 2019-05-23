
TARGET=test
OBJS+= test.o basedevice.o rtc.o string.o
CROSS_COMPILE=arm-linux-
CCFLAG= -fno-builtin
LINK_SCRIPT=test.lds

${TARGET}.bin:${TARGET}
	${CROSS_COMPILE}objcopy -O binary $^ $@

${TARGET}:${OBJS}
	${CROSS_COMPILE}ld -T${LINK_SCRIPT} $^ -o $@

%.o:%.c
	${CROSS_COMPILE}cc -c $^ -o $@ ${CCFLAG}

clean:
	rm -f ${TARGET} ${OBJS} ${TARGET}.bin


