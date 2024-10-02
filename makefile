all: start

start: startgame
	./startgame


startgame: game.o
	gcc game.o -lncurses -o startgame


game.o: game.c
	gcc -c game.c
