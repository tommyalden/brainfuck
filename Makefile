CC = clang
CFLAGS = -Wall -std=c11
SANITIZE = -fsanitize=address,undefined

TARGET = brainfuck
SRC = brainfuck.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

debug: $(SRC)
	$(CC) $(CFLAGS) $(SANITIZE) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET)

.PHONY: all debug clean
