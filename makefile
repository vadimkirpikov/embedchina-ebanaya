all: compile

compile: clean
compile: startgame

startgame: game.o
	gcc game.o -lncurses -o startgame


game.o: game.c
	gcc -c game.c

clean:
	rm -f *.o startgame
