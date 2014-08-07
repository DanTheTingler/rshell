#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

int main(int argc, char *argv[])
{
    while(1) //continue looping
    {
        bool amper = false;
        char str[1024];
        char *user;
        char hostname[256];
        user = getlogin(); //get user's login
        gethostname(hostname, 256); //get user's host name
        cout << user; //user's name
        cout << "@" << hostname; //computer's host name
        cout << "$ "; //bash 
        cin.getline(str, 1024); //get user input

        for(int p  = 0; p < 1024; p++) //check to see if user has enetered a #
        {
            if(str[p] == '#') //if so, set the # to NULL, effectively igorning everything after
            {
                str[p] = '\0';
            }
        }


        if(strcmp(str, "exit") == 0) //check to see if user has typed exit
        {
            exit(1); //if so, exit the program
        }

        int i = 0;
        char *arg[1024]; 

        char *token = strtok(str, " "); //parse input using strtok
        while (token != NULL) 
        {
            arg[i] = token; //put parsed commands into arg
            i++; //incrememnt to the next part of arg
            token = strtok(NULL, " "); //continue parsing
        }

        arg[i] = NULL; //set the end of arg to NULL to avoid seg fault
       
        
        for(int d = 0; d < i; d++) //search for the & 
        {
            if(*arg[d] == '&') //if & is found
            {
                amper = true; //set bool to true
            }
        }

        
        pid_t pid = fork();

        if(pid) //parent
        {
            if(amper == false) //if there was no &, then wait for child process
            {
                wait(0);
            }
        }

        else if(pid == 0) //child process, which means we want to run exec
        {

            if(execvp(arg[0], arg) < 0 ) //calls the command and its flags
            {
                perror("execvp failed: "); //error check/message
            }
            exit(1);
        }

        else //error check for fork: fork has failed, output error message
        {
            perror("fork has failed: ");
            return 0;
        }

    }
    return 0;
}
