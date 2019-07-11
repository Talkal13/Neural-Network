
all: src/main.cpp
	g++ -o bin/main src/main.cpp -Wall -lm -lpthread -lncurses -g