CFLAGS = -Wall -Wextra -pedantic -g
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm
SRC = ./src/main.c
DEST = main

all:
	gcc $(CFLAGS) -o $(DEST) $(SRC) $(LIBS)

clean:
	rm -f ./main
