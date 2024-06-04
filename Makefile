CFLAGS = -Wall -Wextra -pedantic -g
LIBS = -lSDL2 -lSDL2_image

all:
	gcc $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f ./main
