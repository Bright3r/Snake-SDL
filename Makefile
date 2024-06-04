CFLAGS = -Wall -Wextra -pedantic -g
LIBS = -lSDL2 -lSDL2_ttf -lm

all:
	gcc $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f ./main
