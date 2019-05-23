
TARGET=test
OBJS:= $(patsubst %.c, %.o, $(wildcard *.c))
LINK_SCRIPT=test.lds
CROSS_COMPILE=arm-linux
CCFLAGS= -fno-builtin

.PHONY:all clean

all:${TARGET}.bin

${TARGET}.bin:${TARGET}
	${CROSS_COMPILE}-objcopy -O binary $^ $@

${TARGET}:${OBJS}
	${CROSS_COMPILE}-ld -T${LINK_SCRIPT} $^ -o $@

%.o:%.c
	${CROSS_COMPILE}-gcc -c $^ -o $@ ${CCFLAGS}

clean:
	-rm -f ${TARGET}.bin ${TARGET} ${OBJS}

