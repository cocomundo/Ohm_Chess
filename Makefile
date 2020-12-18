# Variables to control Makefile operation

CC = gcc
CFLAGS = -Wall -Wextra -O2

ALL_SOURCE_FILES = *.c
TARGET_TOOL := chess

all: $(TARGET_TOOL)

$(TARGET_TOOL): *.c *.h
	$(CC) $(CFLAGS) -o $(TARGET_TOOL) $(ALL_SOURCE_FILES) -lncurses

clean:
	$(RM) $(TARGET_TOOL) *.o
