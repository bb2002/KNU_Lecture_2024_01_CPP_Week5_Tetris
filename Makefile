FLAGS = -Werror -std=c++11 -g
CC = g++

ifneq ($(OS),Windows_NT)
	FLAGS += -lncurses
endif

all: main

main: main.o game.o tetromino.o console.o utils.o
	$(CC) $(FLAGS) -o main main.o game.o tetromino.o console.o utils.o

console.o: console/console.cpp console/console.h
	$(CC) $(FLAGS) -c -o console.o console/console.cpp

main.o: main.cpp game.h tetromino.h
	$(CC) $(FLAGS) -c -o main.o main.cpp

game.o: game.cpp game.h tetromino.h
	$(CC) $(FLAGS) -c -o game.o game.cpp

tetromino.o: tetromino.cpp tetromino.h
	$(CC) $(FLAGS) -c -o tetromino.o tetromino.cpp

utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) -c -o utils.o utils.cpp

clean:
	rm -f *.o main.exe main
