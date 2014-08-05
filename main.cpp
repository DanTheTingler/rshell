#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
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

    if(execvp(arg[0], arg) < 0 )
    {
        perror("execvp failed: ");
    }



    /*
    if(execl("/bin/ls", "/bin/ls", "-l", NULL) == -1)
    {
        perror("execl failed: ");
    }
    */

    return 0;
}
