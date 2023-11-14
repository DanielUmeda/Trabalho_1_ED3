CC = gcc
CFLAGS = -Wall -std=c99
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))
EXECUTABLE = programaTrab.exe  # Adicione a extensão .exe para executáveis no Windows

all: compile

compile: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: compile
	./$(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE)
