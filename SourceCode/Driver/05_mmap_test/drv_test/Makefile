
C_FILES=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(C_FILES))
TARGET=drv_test
#CROSS_COMPILE=arm-linux-
CROSS_COMPILE=arm-none-linux-gnueabi-
# CROSS_COMPILE=
CFLAGS = -Werror -I../include 

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CROSS_COMPILE)gcc   -o  $@ $^ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

%.o:%.c
	$(CROSS_COMPILE)gcc -c   -o  $@ $<  $(CFLAGS)

.PHONY:all clean

