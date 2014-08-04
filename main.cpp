#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    char str[512];

    cout << "$ ";
    cin >> str;

    if(strcmp(str, "exit") == 0)
    {
        exit(1);
    }

    cout << "Your command was: " << str << endl;

    if(execl("/bin/ls", "-1", NULL) == -1)
    {
        perror("execl failed: ");

    }


    return 0;
}
