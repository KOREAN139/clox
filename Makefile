.SUFFIXES : .c .o

OBJECTS = main.o chunk.o memory.o debug.o value.o vm.o

CC = gcc
CFLAGS = -c

TARGET = clox

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	rm $(TARGET) $(OBJECTS)

main.o : common.h main.c
chunk.o : chunk.h memory.h chunk.c
memory.o : common.h memory.h memory.c
debug.o : debug.h debug.c
value.o : memory.h value.h value.c
vm.o : common.h vm.h vm.c
