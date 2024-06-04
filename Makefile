CFLAGS = -Wall -Wextra -pedantic -g
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm

all:
	gcc $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f ./main
