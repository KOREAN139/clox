.SUFFIXES : .c .o

OBJECTS = main.o chunk.o memory.o debug.o value.o vm.o compiler.o scanner.o

CC = gcc
CFLAGS = -c

TARGET = clox

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	rm $(TARGET) $(OBJECTS)

main.o : common.h memory.h main.c
chunk.o : chunk.h  chunk.c
memory.o : memory.h memory.c
debug.o : debug.h debug.c
value.o : value.h value.c
vm.o : vm.h vm.c
compiler.o : compiler.h compiler.c
scanner.o : scanner.h scanner.c
