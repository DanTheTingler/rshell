all: rshell ls

rshell:
	mkdir ./bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o ./bin/rshell
ls:
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o ./bin/ls
