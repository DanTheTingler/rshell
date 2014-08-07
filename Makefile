all: rshell

rshell:
	mkdir ./bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o ./bin/rshell
