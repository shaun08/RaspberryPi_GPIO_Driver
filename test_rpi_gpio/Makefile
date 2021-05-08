#
# \brief:  Makefile to bulid test file used to test custom GPIO
#          memory device driver for Raspberry Pi 4
# \author: Shaun Prabhu
# \date:   May 08, 2021
#

SRC=test_gpio.c
BIN=test_gpio
CC=gcc
CFLAGS=-Wall
OBJS= $(BIN).o
DEPS= # add header files to build source here

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BIN) $(OBJS)