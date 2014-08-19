rshell
======

THE PROJECT:
This is a bash shell project for hw1-cs100, called rshell. Rshell is a simple program that imitates a simple, linux-based bash command shell. 

ls, which was created for hw2-cs100, emulates the ls command in bash shell, and can take in the -l, -a, and -R flags.

DOCUMENTATION:
Upon startup, rshell prints out the hostname, name of the system currently being used, and the bash sign, '$'. Rshell takes in bash commands through user input and executes as the linux-based bash command shell does. Besides a few bugs detailed below, it's output is nearly identical to a regular bash shell.

ls runs based on user input from argv, so to use it, go into /bin and call ls. From there, you can append directory names and flags to your heart's desire.

BUILDING RSHELL:
Building rshell is easy! A Makefile has been included, and after cloning the repository, simply navigate to where the Makefile is located. A /bin folder will be created for you. Navigate into it, and from there simply run shell by typing "./rshell"!

Building ls is nearly identical to building rshell. Run the makefile, navigate to bin, and there you can utilize ls!


Alas, I am only a student, and there are some bugs with my program. Here they are listed below:

BUGS FOR RSHELL:

1. Can't handle filename with spaces. e.g. cat "this has spaces does not work".

2. Files with quotes are also unable to be handled. 

3. Typing "geany &" usually opens up geany and outputs a [1] 7342, and you can go on typing commands as usual. In my rshell, geany will open and run in the background, but it does not output the [1] 7342. It can, however, continue to accept commands and function normally.

4. The '&' will not work with files that have & in their filename.

5. Running the command echo "hello" will output "hello" instead of the hello that should be output. Difference is the quotes.

6. After running the script command, the user's name will mysteriously disappear while running rshell. This does not occur if the script command is not running.

7. Can't handle IO redirection.

8. Can't handle commands such as ctrl^C, which when typed will exit it out of the shell instead of exiting the current process/command.

BUGS FOR LS:

1. Upon entering a directory that does not exist, it will return a segmentation fault along with the error message. The bash shell's ls only returns the error message.

2. When calling /bin/ls, the color codes for the directories/files & executables don't show up. However, after cd-ing into bin and calling ls, the colors will appear.

3. The spacing for the numbers is off, so the formatting isn't as nice as the ls in bash.

4. The -l command will not accept file names, which is a feature of bash's ls.

5. There are some general formatting issues, such as extra blank lines. Nothing that impairs readibility or usability.

6. The file names are not output in alphabetical order.
