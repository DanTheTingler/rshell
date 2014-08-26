#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <vector>
#include <string>

using namespace std;

string remove_char(const string &input, char to_remove);
void reg(char str[], char *argv[]);
void input(char str[], char *argv[]);
void dinput(char str[], char *argv[]);
void output(char str[], char *argv[]);
void pipe(char *arg[], char *arg2[], bool amper);
void pipeconvert(char str[], char *argv[]);
void pipechaining(char *argv[], bool amper);
void getCurrentPath();
void sig_handler(int signum);
char *allpaths(const char *name);
char **argmodifier(const char *paths);
char *finalpathmod(char *path, char *command);


int main(int argc, char *argv[])
{
    while(1) //continue looping
    {
        bool bcarrot = false;
        bool fcarrot = false;
        bool dcarrot = false;
        bool pole = false;

        char str[1024] = {0};
        char user[256] = {0};
        char hostname[256] = {0};
        
        getlogin_r(user, sizeof(user)-1); //get user's login info
        //{
            //perror("getlogin_r has failed");
        //}    
            
        if(gethostname(hostname, sizeof(hostname)-1) != 0) //get user's host name
        {
            perror("gethostname has failed");
        }

        cout << user; //user's name
        cout << "@" << hostname; //computer's host name
        cout << "$ "; //bash 
        cin.getline(str, 1024); //get user input
        
        //this is for ctrl-C
        if(signal(SIGINT, sig_handler) == SIG_ERR)
        {
            perror("There was an error with signal:");                
        }
    

        //check to see if user has entered special characters
        for(int p  = 0; p < 1024; p++)
        {
            if(str[p] == '#') //if so, set the # to NULL, effectively igorning everything after
            {
                str[p] = '\0';
            }

            if(str[p] == '<')
            {
                bcarrot = true;
            }

            if(str[p-1] != '>' && str[p] == '>' && str[p+1] != '>')
            {
                fcarrot = true;
            }

            if(str[p] == '>' && str[p+1] == '>')
            {
                dcarrot = true;
            }

            if(str[p] == '|')
            {
                pole = true;
            }

        }
	    
        
        //this is for cd
        if(str[0] == 'c' && str[1] == 'd')
        {
            int pos = 0;

            for(int p = 0; p < 1024; p++) //checks to see where the space between cd and the user input is
            {
                if(str[p] == ' ')
                {
                    pos = p;
                }
            }

            char a2[1024] = {0};
        
        
            for(int f = 0, d = pos+1; d < 1024; d++, f++) //this will contain any user input after cd
            {
                a2[f] = str[d];
            }
            
            if(pos == 0) //if there was no space, then that means user will go to home directory
            {
                if(chdir(getenv("HOME")) == -1)
                {
                    perror("Error with chdir");
                }
            }
            else //else cd to wherever they typed
            {
                if(chdir(a2) == -1)
                {
                    perror("Error with chdir");
                }
            }
            
            //this outputs the user's current path
            getCurrentPath();
        }

        else if(strcmp(str, "exit") == 0) //check to see if user has typed exit
        {
            exit(1); //if so, exit the program
        }
	
        
        else if(fcarrot) //if > was typed, call the function for it
        {
            input(str, argv);
        }

        else if(dcarrot) //if >> was typed, call the function for it
        {
            dinput(str, argv);
        }

        else if(bcarrot) //if < was typed, call the function for it
        {
            output(str, argv);
        }

        else if(pole) //if | was typed, call the function for it
        {
            pipeconvert(str, argv);
        }
        
        else //call the regular one
        {
            reg(str, argv);
        }

        continue;

    }

    return 0;
}

//this function is the sig_handler for ctrl-C, makes sure that the program won't exit
void sig_handler(int signum)
{
    if(signal(SIGINT,SIG_IGN) == SIG_ERR)
    {
        perror("There was an error with signal");
    }
    cout << endl;
}

//function used to get the current working directory/path
void getCurrentPath()
{
    char path[1024] = {0};
    if(getcwd(path, sizeof(path)) != NULL)
    {
        cout << path << endl;
    }
    else //error checking
    {
        perror("There was an error with getcwd");
    }
}

//function used to get the path so we can run user input
char *allpaths(const char *name)
{
    char *pathing = getenv(name);
    if(pathing == NULL)
    {
        perror("There was a problem with getenv");
    }
    return pathing;
}

//converts the path so that it will be useable
char **argmodifier(char *paths)
{
    int d = 0;

    char **argm = new char* [2048];

    char *tokens = strtok(paths, " :"); //parse input using strtok
    while (tokens != NULL) 
    {
        argm[d] = tokens; //put parsed commands into arg
        d++; //incrememnt to the next part of arg
        tokens = strtok(NULL, " :"); //continue parsing
    }

    argm[d] = NULL; //set the end of arg to NULL to avoid seg fault
 
    return argm;
}

//this combines the user input with the path
char *finalpathmod(char *path, char *command)
{
    char *finalpath = new char[1024];
    strcpy(finalpath, path);
    strcat(finalpath, "/");
    strcat(finalpath, command);

    return finalpath;

}


//function used to remove spaces for a char array
string remove_char(const string &input, char to_remove)
{
    string output;
    for(unsigned i=0; i<input.size(); i++)
    {
        if(input[i]!=to_remove)
        output+=input[i];
    }
    
    return output;
}


//function used if no special io redirection/piping are used
void reg(char str[], char *argv[])
{
    bool succeed = false;
    char *pathing = allpaths("PATH");
    char cpathing[1024] = {0};

    strcpy(cpathing, pathing);

    char **westbrook = argmodifier(cpathing);
    char *kd = new char[1024];

    bool amper = false;
    int i = 0;

    char **arg;
    arg = new char* [1024];

    char *token = strtok(str, " "); //parse input using strtok
    while (token != NULL) 
    {
        arg[i] = token; //put parsed commands into arg
        i++; //incrememnt to the next part of arg
        token = strtok(NULL, " "); //continue parsing
    }

    //search for the various needed elements
    for(int d = 0; d < i; d++)
    {
        if(*arg[d] == '&') //if & is found
        {
            amper = true; //set bool to true
        }
    }

    
    pid_t pid = fork(); //error checking is in the else

    if(pid > 0) //parent
    {
        if(amper == false) //if there was no &, then wait for child process
        {
            wait(0);
        }
    }

    else if(pid == 0) //child process, which means we want to run exec
    {
        
        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg[0]);
            if(execv(kd, arg) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }
        }

        if(!succeed)
        {
            perror("Error with execv");
        }

        exit(1);
    }

    else //error check for fork: fork has failed, output error message
    {
        perror("fork has failed: ");
        exit(1);
    }

    //delete[] westbrook;
    delete[] arg;
}

//function used if the > is called
void input(char str[], char *argv[])
{
    bool succeed = false;
    char *pathing = allpaths("PATH");
    char cpathing[1024] = {0};

    strcpy(cpathing, pathing);

    char **westbrook = argmodifier(cpathing);
    char *kd = new char[1024];
    

    bool amper = false;
    int i = 0;
    int pos = 0;

    //split str depending on where the > is.
    for(int p = 0; p < 1024; p++)
    {
        if(str[p] == '>')
        {
            pos = p;
        }
    }

    char a1[1024] = {0};
    char a2[1024] = {0};
    
    for(int d = 0; d < pos; d++)
    {
        a1[d] = str[d];
    }

    
    for(int f = 0, d = pos+1; d < 1024; d++, f++)
    {
        a2[f] = str[d];
    }
    
    int alength = sizeof(a2);
    a2[alength-1] = '\0';
    
     
    //convert the file name from char array to a string
    string filenamet(a2, 1024); 

    
    char **arg;
    arg = new char* [1024];
    
    char *token = strtok(a1, " "); //parse input using strtok
    while (token != NULL) 
    {
        arg[i] = token; //put parsed commands into arg
        i++; //incrememnt to the next part of arg
        token = strtok(NULL, " "); //continue parsing
    }

    arg[i] = NULL; //set the end of arg to NULL to avoid seg fault
    
    //search for the various needed elements
    for(int d = 0; d < i; d++)
    {
        if(*arg[d] == '&') //if & is found
        {
            amper = true; //set bool to true
        }
    }

    //string test = "result";
    string filename = filenamet;
    filename = remove_char(filename, ' ');

    //now we fork
    pid_t pid = fork();

    if(pid > 0) //parent
    {
        if(amper == false) //if there was no &, then wait for child process
        {
            wait(0);
        }
    }

    else if(pid == 0) //child process, which means we want to run exec
    {
        //call open on file
        int fd = open(filename.c_str(),O_RDWR|O_CREAT|O_TRUNC,0666);
        
        if(fd == -1)
        {
            perror("Error with opening file");
            exit(1);
        }
       
        if(close(1) == -1)
        {
            perror("Error with close");
        }

        if(dup2(fd, 1) == -1)
        {
            perror("There was a problem with dup2");
        }

        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg[0]);
            if(execv(kd, arg) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }

            
        }

        if(!succeed)
        {
            perror("Error with execv");
        }
       
        exit(1);
    }

    else //error check for fork: fork has failed, output error message
    {
        perror("fork has failed");
        exit(1);
    }

    delete[] arg;

}

//function used if the >> is called
void dinput(char str[], char *argv[])
{
    bool succeed = false;
    char *pathing = allpaths("PATH");
    char cpathing[1024] = {0};

    strcpy(cpathing, pathing);

    char **westbrook = argmodifier(cpathing);
    char *kd = new char[1024];

    bool amper = false;
    int i = 0;
    int pos = 0, pos1 = 0;

    //split str depending on where the > is.
    for(int p = 0; p < 1024; p++)
    {
        if(str[p] == '>' && str[p+1] == '>')
        {
            pos = p;
            pos1 = p+1;
        }
    }
    

    char a1[1024] = {0};
    char a2[1024] = {0};
    
    for(int d = 0; d < pos; d++)
    {
        a1[d] = str[d];
    }

    
    for(int f = 0, d = pos1+1; d < 1024; d++, f++)
    {
        a2[f] = str[d];
    }
    
    int alength = sizeof(a2);
    a2[alength-1] = '\0';
    
     
    //convert the file name from char array to a string
    
    string filenamet(a2, 1024); 

    
    char **arg;
    arg = new char* [1024];
    
    /*
    char **filename;
    filename = new char* [1024];
    */
    char *token = strtok(a1, " "); //parse input using strtok
    while (token != NULL) 
    {
        arg[i] = token; //put parsed commands into arg
        i++; //incrememnt to the next part of arg
        token = strtok(NULL, " "); //continue parsing
    }

    arg[i] = NULL; //set the end of arg to NULL to avoid seg fault
   

    //search for the various needed elements
    for(int d = 0; d < i; d++)
    {
        if(*arg[d] == '&') //if & is found
        {
            amper = true; //set bool to true
        }
    }

    string filename = filenamet;
    //remove all whitespace
    filename = remove_char(filename, ' ');
    //now we fork
    pid_t pid = fork();

    if(pid > 0) //parent
    {
        if(amper == false) //if there was no &, then wait for child process
        {
            wait(0);
        }
    }

    else if(pid == 0) //child process, which means we want to run exec
    {
        //call open on file
        int fd = open(filename.c_str(),O_RDWR|O_CREAT|O_APPEND,0666);
        
        if(fd == -1)
        {
            perror("Error with opening file");
            exit(1);
        }
        
        if(dup2(fd, 1) == -1)
        {
            perror("There was a problem with dup2:");
        }

        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg[0]);
            if(execv(kd, arg) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }

            
        }

        if(!succeed)
        {
            perror("Error with execv");
        }
       
        exit(1);
    }

    else //error check for fork: fork has failed, output error message
    {
        perror("fork has failed: ");
        exit(1);
    }
    
    delete[] arg;
}

//function used if < is called
void output(char str[], char *argv[])
{
    bool succeed = false; //bool used to check if execv fails later
    char *pathing = allpaths("PATH"); //this stores the contents of PATH into a char pointer pathing
    char cpathing[1024] = {0}; //convert pathing to a char array

    strcpy(cpathing, pathing); //copy pathing into cpathing

    char **westbrook = argmodifier(cpathing); //call argmodifier on cpathing to conver it
    char *kd = new char[1024]; //kd will be used later


    bool amper = false;
    int i = 0;
    int pos = 0;

    //split str depending on where the > is.
    for(int p = 0; p < 1024; p++)
    {
        if(str[p] == '<')
        {
            pos = p;
        }
    }
    
    char a1[1024] = {0};
    char a2[1024] = {0};
    
    for(int d = 0; d < pos; d++)
    {
        a1[d] = str[d];
    }

    
    for(int f = 0, d = pos+1; d < 1024; d++, f++)
    {
        a2[f] = str[d];
    }
    
    int alength = sizeof(a2);
    a2[alength-1] = '\0';
    
     
    //convert the file name from char array to a string
    
    string filenamet(a2, 1024); 

    
    char **arg;
    arg = new char* [1024];
    
    char *token = strtok(a1, " "); //parse input using strtok
    while (token != NULL) 
    {
        arg[i] = token; //put parsed commands into arg
        i++; //incrememnt to the next part of arg
        token = strtok(NULL, " "); //continue parsing
    }

    arg[i] = NULL; //set the end of arg to NULL to avoid seg fault
   

    //search for the various needed elements
    for(int d = 0; d < i; d++)
    {
        if(*arg[d] == '&') //if & is found
        {
            amper = true; //set bool to true
        }
    }

    //string test = "result";
    string filename = filenamet;
    filename = remove_char(filename, ' ');

    //now we fork
    pid_t pid = fork();

    if(pid > 0) //parent
    {
        if(amper == false) //if there was no &, then wait for child process
        {
            if(wait(0) == -1)
            {
                perror("Wait has failed");
            }
        }
    }

    else if(pid == 0) //child process, which means we want to run exec
    {
        //call open on file
        int fd = open(filename.c_str(),O_RDONLY);
        
        if(fd == -1) //error checking on open
        {
            perror("Error with opening file");
            exit(1);
        }

        if(close(0) == -1)
        {
            perror("Error with close");
        }

        int fd2 = dup(fd);
        if(fd2 == -1)
        {
            perror("There was an error with dup");
        }

        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg[0]);
            if(execv(kd, arg) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }
            
        }

        if(!succeed)
        {
            perror("Error with execv");
        }

       
        exit(1);
    }

    else //error check for fork: fork has failed, output error message
    {
        perror("fork has failed: ");
        exit(1);
    }

    delete[] arg;

}

//this function is to change the user input to be suitable for piping
void pipeconvert(char str[], char *argv[])
{
    bool amper = false;
    int i = 0, b = 0;
    int pos = 0;

     
    //split str depending on where the > is.
    for(int p = 0; p < 1024; p++)
    {
        if(str[p] == '|')
        {
            pos = p;
        }
    }
    

    char a1[1024] = {0};
    char a2[1024] = {0};
    
    for(int d = 0; d < pos; d++)
    {
        a1[d] = str[d];
    }

    
    for(int f = 0, d = pos+1; d < 1024; d++, f++)
    {
        a2[f] = str[d];
    }
    
    int alength = sizeof(a2);
    a2[alength-1] = '\0';
    
    
    //this is for the first part of the user input, before |
    char **arg;
    arg = new char* [1024];
    
    //this is for the second part of the user input, after |
    char **arg2;
    arg2 = new char* [1024];
    
    
    char *token = strtok(a1, " "); //parse input using strtok
    while (token != NULL) 
    {
        arg[i] = token; //put parsed commands into arg
        i++; //incrememnt to the next part of arg
        token = strtok(NULL, " "); //continue parsing
    }

    arg[i] = NULL; //set the end of arg to NULL to avoid seg fault
   
    
    char *token1 = strtok(a2, " >|<"); //parse input using strtok
    while (token1 != NULL) 
    {
        arg2[b] = token1; //put parsed commands into arg
        b++; //incrememnt to the next part of arg
        token1 = strtok(NULL, " >|<"); //continue parsing
    }

    arg2[b] = NULL; //set the end of arg to NULL to avoid seg fault
    

    //search for the various needed elements
    for(int c = 0; c < i; c++)
    {
        if(*arg[c] == '&') //if & is found
        {
            amper = true; //set bool to true
        }
    }

    //call pipe, which will call pipechaining
    pipe(arg, arg2, amper);

    delete[] arg;
    delete[] arg2;
}

//function used if | is called, second part of the two piping functions
void pipechaining(char *arg2[], bool amper)
{
    bool succeed = false;
    char *pathing = allpaths("PATH");
    char cpathing[1024] = {0};

    strcpy(cpathing, pathing);

    char **westbrook = argmodifier(cpathing);
    char *kd = new char[1024];


    int pid2 = fork();
    if(pid2 == -1) //error checking for the fork
    {
        perror("Error with the second fork:");
    }

    else if(pid2 == 0) //child function
    {
        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg2[0]);
            if(execv(kd, arg2) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }
            
        }

        if(!succeed)
        {
            perror("Error with execv");
        }
        
        exit(1);
    }
    
    if(amper) //wait if the & was entered
    {
        if(wait(0) == -1)
        {
            perror("Error with wait:");
        }
    }
    
}

//function used if | is called
void pipe(char *arg[], char *arg2[], bool amper)
{
    bool succeed = false;
    char *pathing = allpaths("PATH");
    char cpathing[1024] = {0};

    strcpy(cpathing, pathing);

    char **westbrook = argmodifier(cpathing);
    char *kd = new char[1024];


    
    //PIPING STARTS HERE
    int fd[2];
    if(pipe(fd) == -1)
    {
        perror("Error with pipe1");
    }
    
    //now we fork
    pid_t pid = fork();
    
    if(pid == -1) //error check for fork: fork has failed, output error message
    {
        perror("fork1 has failed");
        exit(1);
    }
    
    else if(pid == 0) //child process, which means we want to run exec
    {
        if(close(fd[0]) == -1) //close stdin(0)
        {
            perror("Error with close1");
        }

        if(dup2(fd[1],1) == -1) //set stdout to the pipe
        {
            perror("Error with dup2");
        }
    
        //call execvp on the first part
        for(int f = 0; westbrook[f] != NULL; f++)
        {
            kd = finalpathmod(westbrook[f], arg[0]);
            if(execv(kd, arg) == -1) //if execv wasn't successful then keep going
            {
                continue;
            }
            else //if execv is successful, no need to check the other paths anymore
            {
                succeed = true;
                break;
            }
            
        }

        if(!succeed)
        {
            perror("Error with execv");
        }
        
        exit(1);
    }


    //close stdout
    if(close(fd[1]) == -1)
    {
        perror("Error with close2");
    }
    
    //this is for the second part of the command
    
    //used later to restore stdin
    int in = dup(0);

    
    if(in == -1)
    {
        perror("Error with 2nd dup");
    }

    //set stdin to be pipe for the latter part of the command
    if(dup2(fd[0], 0) == -1)
    {
        perror("Error with the 3rd dup:");
    }
    
    //wait at the very end
    if(wait(0) == -1)
    {
        perror("Error with wait");
    }
   
    pipechaining(arg2, amper);
    
    //restore stdin
    if(dup2(in, 0) == -1)
    {
        perror("Error with dup2");
    }
    cout << flush;
}
