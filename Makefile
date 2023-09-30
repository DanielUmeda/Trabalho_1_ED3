CC = gcc
CFLAGS = -Wall -std=c99
OBJ = src/main.o src/dados.o src/tecnologia.o

all: meu_programa

meu_programa: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o meu_programa
