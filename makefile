CC = g++
CFLAGS = -std=c++11 -Wall
LFLAGS =

TARGET = program

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
