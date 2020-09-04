# Variables to control Makefile operation

CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET_TOOL := chess

all: $(TARGET_TOOL)

# Targets, needed to bring the executable up to date

$(TARGET_TOOL): main.o move_gen.o evaluation.o board.o
	$(CC) $(CFLAGS) -o $(TARGET_TOOL) main.o move_gen.o evaluation.o board.o -lncurses
main.o: main.c move_gen.h board.h
	$(CC) $(CFLAGS) -c main.c 
move_gen.o: move_gen.h evaluation.h board.h

evaluation.o: evaluation.h

board.o: board.h

clean:
	$(RM) $(TARGET_TOOL) *.o

# $(TARGET_TOOL): eeprodprog.o
#	$(CC) $(CFLAGS) eeprodprog.c -o $@ -li2c
