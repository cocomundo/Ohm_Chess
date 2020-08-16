# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -O2

TARGET_TOOL := chess

all: $(TARGET_TOOL)

# Targets, needed to bring the executable up to date

$(TARGET_TOOL): main.o move_gen.o evaluation.o board.o
	$(CXX) $(CXXFLAGS) -o $(TARGET_TOOL) main.o move_gen.o evaluation.o board.o
main.o: main.cpp move_gen.h board.h
	$(CXX) $(CXXFLAGS) -c main.cpp 
move_gen.o: move_gen.h evaluation.h board.h

evaluation.o: evaluation.h

board.o: board.h

clean:
	$(RM) $(TARGET_TOOL) *.o

# $(TARGET_TOOL): eeprodprog.o
#	$(CC) $(CFLAGS) eeprodprog.c -o $@ -li2c
