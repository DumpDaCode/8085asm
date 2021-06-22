CC=gcc
CFLAGS=lm
OBJ=main.o assembler.o file_structures.o instruction_set.o directives.o
TARGET=8085asm

$(TARGET): all clear

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

clear:
	rm -rf *.o

%.o: %.c
	$(CC) -c $^ -o $@ 
