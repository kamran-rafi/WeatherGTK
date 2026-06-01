CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = main.c src/app.c src/utils/*.c src/core/*.c src/ui/*.c src/ui/components/*.c src/ui/pages/*.c
OUT = main.out

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `pkg-config --cflags --libs gtk4` -lcurl

run: all
	./$(OUT)

clean:
	rm -rf build