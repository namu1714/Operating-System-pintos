CC = gcc
TARGET = testlib
OBJECTS = main.o list.o hash.o hex_dump.o
CFLAGS = -W -Wall

all: $(TARGET)

list.o: list.c list.h
	$(CC) -c list.c $(CFLAGS)

hash.o: hash.c hash.h
	$(CC) -c hash.c $(CFLAGS)

hex_dump.o: hex_dump.c hex_dump.h round.h
	$(CC) -c hex_dump.c $(CFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

clean:
	rm $(OBJECTS) $(TARGET) 
