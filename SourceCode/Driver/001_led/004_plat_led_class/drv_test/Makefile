
C_FILES=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(C_FILES))
TARGET=drv_test
CROSS_COMPILE=arm-linux-
# CROSS_COMPILE=
CFLAGS = -Werror -I../include -march=armv4

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CROSS_COMPILE)gcc  $(CFLAGS) -o  $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

%.o:%.c
	$(CROSS_COMPILE)gcc $(CFLAGS)  -c -o  $@ $< 

.PHONY:all clean

