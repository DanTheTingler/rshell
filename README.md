rshell
======

THE PROJECT:
This is a bash shell project for hw1-cs100, called rshell. Rshell is a simple program that imitates a simple, linux-based bash command shell.

DOCUMENTATION:
Upon startup, rshell prints out the hostname, name of the system currently being used, and the bash sign, '$'. Rshell takes in bash commands through user input and executes as the linux-based bash command shell does. Besides a few bugs detailed below, it's output is nearly identical to a regular bash shell.

BUILDING RSHELL:
Building rshell is easy! A Makefile has been included, and after cloning the repository, simply navigate to where the Makefile is located. A /bin folder will be created for you. Navigate into it, and from there simply run shell by typing "./rshell"!

BUGS:
Alas, I am only a student, and there are some bugs with my program. Here they are listed below:

1. Can't handle filename with spaces. e.g. cat "this has spaces does not work".

2. Files with quotes are also unable to be handled. 

3. Typing "geany &" usually opens up geany and outputs a [1] 7342, and you can go on typing commands as usual. In my rshell, geany will open and run in the background, but it does not output the [1] 7342. It can, however, continue to accept commands and function normally.

4. The '&' will not work with files that have & in their filename.

5. Running the command echo "hello" will output "hello" instead of the hello that should be output. Difference is the quotes.
