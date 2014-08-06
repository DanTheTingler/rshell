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
    bool amper = false;
    char str[256];
    char *user;
    user = getlogin();
    cout << user;
    cout << "$ ";
    cin.getline(str, 256);

    if(strcmp(str, "exit") == 0)
    {
        exit(1);
    }

    cout << "Your command was: " << str << endl;

    //testing out input parsing


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
        return 0;
    }

    if(pid == 0)
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

    return 0;
}
