all: bin rshell ls

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	g++ -Wall -Werror -ansi -pedantic src/rshell.cpp -o ./bin/rshell
ls:
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o ./bin/ls
