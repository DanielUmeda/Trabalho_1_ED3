CC = gcc
CFLAGS = -Wall -std=c99
SRC_FILES = $(wildcard *.c)  # Todos os arquivos .c no diretório atual
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))
EXECUTABLE = programaTrab

all: compile

compile: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: compile
	./$(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE)
