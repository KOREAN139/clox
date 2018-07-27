.SUFFIXES : .c .o

OBJECTS = main.o scanner.o debug.o

CC = gcc
CFLAGS = -c

TARGET = clox

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	rm $(TARGET) $(OBJECTS)

main.o : scanner.h debug.h main.c
scanner.o : scanner.h scanner.c
debug.o : debug.h debug.c

