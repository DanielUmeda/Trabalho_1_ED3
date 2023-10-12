CC = gcc
CFLAGS = -Wall -std=c99
SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, src/%.o, $(SRC_FILES))
EXECUTABLE = programaTrab

all: compile

compile: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: compile
	./$(EXECUTABLE)

clean:
	rm -f src/*.o $(EXECUTABLE)
