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
    while(1)
    {
        bool amper = false;
        char str[1024];
        char *user;
        user = getlogin();
        cout << user;
        cout << "$ ";
        cin.getline(str, 1024);

        for(int p  = 0; p < 1024; p++)
        {
            if(str[p] == '#')
            {
                str[p] = '\0';
            }
        }


        if(strcmp(str, "exit") == 0)
        {
            exit(1);
        }

        int i = 0;
        char *arg[64];

        char *token = strtok(str, " ");
        while (token != NULL)
        {
            arg[i] = token;
            i++;
            token = strtok(NULL, " ");
        }

        arg[i] = NULL;
       
        
        for(int d = 0; d < i; d++)
        {
            if(*arg[d] == '&')
            {
                amper = true;
            }
        }

        
        pid_t pid = fork();

        if(pid) //parent
        {
            if(amper == false)
            {
                wait(0);
            }
        }

        else if(pid == 0) //child
        {

            if(execvp(arg[0], arg) < 0 )
            {
                perror("execvp failed: ");
            }
            exit(1);
        }

        else
        {
            //the fork will have failed
            perror("fork has failed: ");
            return 0;
        }

        /*
        if(execl("/bin/ls", "/bin/ls", "-l", NULL) == -1)
        {
            perror("execl failed: ");
        }
        */
    }
    return 0;
}
