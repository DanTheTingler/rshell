#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>


using namespace std;

int main()
{
    char str[512];

    cout << "$ ";
    cin >> str;

    cout << "Your command was: " << str << endl;


    return 0;
}
