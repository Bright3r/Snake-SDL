CFLAGS = -Wall -Wextra -pedantic -g -I./src/include
LDFLAGS = -L./src
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm -pthread
SRC = ./src/main.c ./src/apple.c ./src/snake.c ./src/llist.c ./src/queue.c ./src/network.c ./src/ui.c ./src/utils.c
DEST = main

all:
	gcc $(CFLAGS) $(SRC) $(LDFLAGS) $(LIBS) -o $(DEST)

clean:
	rm -f $(DEST)
