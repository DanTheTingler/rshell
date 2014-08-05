#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    char str[256];
    cout << "$ ";
    cin.getline(str, 256);

    if(strcmp(str, "exit") == 0)
    {
        exit(1);
    }

    cout << "Your command was: " << str << endl;

    //testing out input parsing

    char *token = strtok(str, " ");
    while (token != NULL)
    {
        cout << token << endl;
        token = strtok(NULL, " ");
    }


    if(execl("/bin/ls", "/bin/ls", "-l", NULL) == -1)
    {
        perror("execl failed: ");
    }
    

    return 0;
}
