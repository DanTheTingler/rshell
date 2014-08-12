#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <vector>

using namespace std;


void printtime(const struct stat &buf) //prints the time
{
    time_t t = buf.st_mtime;
    tm * tminfo = localtime(&t);
    char buffer[50]; //c-string buffer of size 50

    strftime(buffer, 50, "%b %d %R", tminfo);
    cout << buffer << " "; //puts prints the content of the c-string, which in this case contains the time

}

void printall(const struct stat &buf) //prints the mode, then prints read, write, execute permissions
{

    (buf.st_mode & S_IFREG) ? cout<< '-':
    (buf.st_mode & S_IFDIR) ? cout<< 'd': //directory
    (buf.st_mode & S_IFLNK) ? cout<< 'l': //link
    (buf.st_mode & S_IFSOCK) ? cout<<'s':
    (buf.st_mode & S_IFBLK) ? cout<< 'b':
    (buf.st_mode & S_IFIFO) ? cout<< 'f':
    (buf.st_mode & S_IFCHR) ? cout<< 'c':
    cout<<'-';


    //user
    (buf.st_mode & S_IRUSR) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWUSR) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXUSR) ? cout << 'x' : cout << '-';

    //group
    (buf.st_mode & S_IRGRP) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWGRP) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXGRP) ? cout << 'x' : cout << '-';

    //others
    (buf.st_mode & S_IROTH) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWOTH) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXOTH) ? cout << 'x' : cout << '-';


    cout << " ";
}

void output(struct stat buf, dirent *direntp) //this function outputs the name of direntp with color!! (Extra Credit)
{
    if(buf.st_mode & S_IXUSR)//executable (green)
    {
        cout<<"\033[38;5;34m"<<direntp->d_name<<"\033[0;00m" << " ";
    }   

    else if(buf.st_mode & S_IFDIR)//directory (blue)
    {
        cout<<"\033[38;5;32m"<<direntp->d_name<<"\033[0;00m" << " ";
    }

    else if(direntp->d_name[0] == '.') //hidden files (gray background)
    {
        cout<<"\033[47m"<<direntp->d_name<<"\033[0;00m" << " ";
    }   

    else if(direntp->d_name[0] == '.' && (buf.st_mode & S_IFDIR)) //hidden directory (gray background, blue words)
    {
        cout<<"\033[47m\033[38;5;32m"<<direntp->d_name<<"\033[0;00m" << " ";
    }

    else if(direntp->d_name[0] == '.' && (buf.st_mode & S_IXUSR)) //hidden executable (gray background, green words)
    {
        cout<<"\033[47m\033[38;5;34m"<<direntp->d_name<<"\033[0;00m" << " ";
    }
    
    else //none of the above cases, cout as normal
    {
        cout<<direntp->d_name << " ";
    }   
}


int noflag(string dirName) //this function is for when the user inputs no flags
{
    if((strcmp(dirName.c_str(), "")) == 0) //if user did not specify a file/directory
    {
        dirName = "."; //default name
    }

    DIR *dirp;
    dirent *direntp;
    

    if(!(dirp = opendir(dirName.c_str()))) //opendir and error checking
    {
        perror("Error with opendir: ");
    }
    
    while ((direntp = readdir(dirp))) //readdir function, error checking inside loop
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        //modify the directory name so that it's usable, append on the necessary parts
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if((stat(filepath , &buf)) == -1) //stat function & error checking
        {
            perror("There is an error with stat: ");
        }
        
        if(direntp->d_name[0] == '.') //if the file name starts with a . continue on
        {
            continue;
        }

        //filename
        //cout << direntp->d_name << " ";
        output(buf, direntp);
    }
    
    cout << endl;
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }
    
    return 0;
}
    
int aflag(string dirName) //this function is for when the user inputs ONLY the -a flag
{ 
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    
    DIR *dirp;
    
    dirent *direntp;
    

    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir: ");
    }
    
    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if((stat(filepath , &buf)) == -1)
        {
            perror("There is an error with stat: ");
        }
        
        output(buf, direntp); 
    //note there is no continue here, as we want the . files
    }
    
    cout << endl;
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }

    return 0;
}


// for just the l-flag
int lflag(string dirName)
{ 
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    
    DIR *dirp;
    
    dirent *direntp;
    


    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir: ");
    }
    
    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if((stat(filepath , &buf)) == -1)
        {
            perror("There is an error with stat: ");
        }
        
        struct passwd *pwd; //used later to get user id
        string userid;

        struct group *gid; //used later to get group id
        string groupid;

        int size = 0;

        if(direntp->d_name[0] == '.')
        {
            continue;
        }
        
        //have to print out extra info for the -l flag

        //prints mode and file permissions
        printall(buf); 
            
        //number of links
        cout << buf.st_nlink << " ";
        
        if(!(pwd = getpwuid(buf.st_uid)))
        {
            perror("Error with getpwuid");
        }

        userid = pwd->pw_name;
        cout << userid << " ";
            
        //gid
        if(!(gid = getgrgid(buf.st_gid)))
        {
            perror("Error with getgrgid");
        }   
        
        groupid = gid->gr_name;
        cout << groupid << " ";   
            
        //size in bytes
        size = buf.st_size;
        cout << size << " ";
           
        //month, date, time
        printtime(buf); 

        //output file/directory name
        output(buf, direntp);
        cout << endl;
    }
    
    cout << endl;
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }

    return 0;
}

int rflag(string dirName) //this is for recursion, where the user inputs -R
{

    vector<char*> direc; //vector that will store directory names for recursion	
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    DIR *dirp;
    dirent *direntp;

    if(!(dirp = opendir(dirName.c_str())))
    {
	    perror("Error with opendir");
    }
 
       
    if(strcmp(dirName.c_str(), ".") == 0)
    {
        cout << ".:" << endl;
    }
    
    else
    {
        cout << dirName << ":" << endl;
    }
    
    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        } 
        
        struct stat buf;

        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if((stat(filepath , &buf)) == -1)
        {
            perror("There is an error with stat: ");
        }

        if ( direntp->d_name[0] == '.' )
        {
            continue;
        }

        output(buf, direntp); //output all file names first
        
        //if its a directory, push into vector for recursion later
        if(S_ISDIR(buf.st_mode))
        {
		    direc.push_back(direntp->d_name); 
	    }

    }
    cout << endl;
    cout << endl;
 
    //this is the recursive portion
    int dsize = direc.size();
    for(int d = 0; d < dsize; d++) //for each directory in the vectory, call rflag again
    { 
        rflag(dirName+"/"+ direc.at(d)); //append the necessary parts so that it can be called
    }

    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }
    
    return 0;
}


//for the laflag, which is when the user calls -l -a or any combination of the two
int laflag(string dirName)
{ 
    DIR *dirp;
    
    dirent *direntp;
    
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }

    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir: ");
    }
    
    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if(stat(filepath, &buf) == -1)
        {
            perror("Error with laflag stat: ");
        }
        
        struct passwd *pwd;
        string userid;

        struct group *gid;
        string groupid;

        int size = 0;


        //prints mode and file permissions
        printall(buf); 
            
        //number of links
        cout << buf.st_nlink << " ";
        
        if(!(pwd = getpwuid(buf.st_uid)))
        {
            perror("Error with getpwuid");
        }

        userid = pwd->pw_name;
        cout << userid << " ";
            
        //gid
        if(!(gid = getgrgid(buf.st_gid)))
        {
            perror("Error with getgrgid");
        }   
        
        groupid = gid->gr_name;
        cout << groupid << " ";
            
        //size in bytes
        size = buf.st_size;
        cout << size << " ";
           
        //month, date, time
        printtime(buf); 

        //filename
        output(buf, direntp);
        cout << endl;
    }
    
    cout << endl;
   
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }

    return 0;
}

int raflag(string dirName) //for when the user calls the -r -a flag, or any combination of them
{
    vector<char*> direc;	
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    DIR *dirp;
    
    dirent *direntp;

    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir");
    }
 
       
    if(strcmp(dirName.c_str(), ".") == 0)
    {
        cout << ".:" << endl;
    }
    
    else
    {
        cout <<  dirName << ":" << endl;
    }
    
    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if((stat(filepath , &buf)) == -1)
        {
            perror("There is an error with stat: ");
        }

        output(buf, direntp);

        if ( direntp->d_name[0] == '.' )
        {
            continue;
        }

        if(S_ISDIR(buf.st_mode))
        {
            direc.push_back(direntp->d_name);
        }

    }
    cout << endl;
    cout << endl;
 
    int dsize = direc.size();
    for(int d = 0; d < dsize; d++)
    { 
        raflag(dirName+"/"+ direc.at(d));
    }
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }
    
    return 0;
}

int rlflag(string dirName) //this is for when the user inputs the -r and -l flag, or any combination
{
    vector<char*> direc;	
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    DIR *dirp;
    
    dirent *direntp;

    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir: ");
    } 
       
    if(strcmp(dirName.c_str(), ".") == 0)
    {
        cout << ".:" << endl;
    }
    
    else
    {
        cout << dirName << ":" << endl;
    }

    while ((direntp = readdir(dirp)))
    {
        //cout << direntp->d_name << endl; 
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if(stat(filepath, &buf) == -1)
        {
            perror("Error with laflag stat: ");
        }
    
        struct passwd *pwd;
        string userid;

        struct group *gid;
        string groupid;

        int size = 0;

        if ( direntp->d_name[0] == '.' )
        {
            continue;
        }
        
         //prints mode and file permissions
        printall(buf); 
            
        //number of links
        cout << buf.st_nlink << " ";
            
        //uid
        if(!(pwd = getpwuid(buf.st_uid)))
        {
            perror("Error with getpwuid");
        }

        userid = pwd->pw_name;
        cout << userid << " ";
            
        //gid
        if(!(gid = getgrgid(buf.st_gid)))
        {
            perror("Error with getgrgid");
        }
        groupid = gid->gr_name;
        cout << groupid << " ";
            
        //size in bytes
        size = buf.st_size;
        cout << size << " ";
           
        //month, date, time
        printtime(buf); 

        output(buf, direntp);
        cout << endl;
        
        //push back into the vector if its a directory
        if(S_ISDIR(buf.st_mode))
        {
		    direc.push_back(direntp->d_name);
        }

    }
    cout << endl;
    cout << endl;
 
    int dsize = direc.size();
    for(int d = 0; d < dsize; d++)
    {
        rlflag(dirName+"/"+ direc.at(d));
    } 
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }
    
    return 0;
}

int rlaflag(string dirName) //this is for when the user inputs all 3 flags
{
    vector<char*> direc;	
    if((strcmp(dirName.c_str(), "")) == 0)
    {
        dirName = ".";
    }
    
    DIR *dirp;
    
    dirent *direntp;

    if(!(dirp = opendir(dirName.c_str())))
    {
        perror("Error with opendir: ");
    } 
       
    if(strcmp(dirName.c_str(), ".") == 0)
    {
        cout << ".:" << endl;
    }
    
    else
    {
        cout << dirName << ":" << endl;
    }

    while ((direntp = readdir(dirp)))
    {
        if(errno != 0)
        {
            perror("Error with readdir");
        }
        
        struct stat buf;
        char filepath[1024];
        strcpy(filepath, dirName.c_str());
        strcat(filepath, "/");
        strcat(filepath, direntp->d_name);

        if(stat(filepath, &buf) == -1)
        {
            perror("Error with laflag stat: ");
        }
    
        struct passwd *pwd;
        string userid;

        struct group *gid;
        string groupid;

        int size = 0;

        //prints mode and file permissions
        printall(buf); 
            
        //number of links
        cout << buf.st_nlink << " ";
            
        //uid
        if(!(pwd = getpwuid(buf.st_uid)))
        {
            perror("Error with getpwuid");
        }

        userid = pwd->pw_name;
        cout << userid << " ";
            
        //gid
        if(!(gid = getgrgid(buf.st_gid)))
        {
            perror("Error with getgrgid");
        }
        groupid = gid->gr_name;
        cout << groupid << " ";
            
        //size in bytes
        size = buf.st_size;
        cout << size << " ";
           
        //month, date, time
        printtime(buf); 

        output(buf, direntp);
        cout << endl;

        if( direntp->d_name[0] == '.' )
        {
            continue;
        } 
       
        if(S_ISDIR(buf.st_mode))
        {
		    direc.push_back(direntp->d_name);
        }

    }
    cout << endl;
    cout << endl;
 
    int dsize = direc.size();
    for(int d = 0; d < dsize; d++)
    {
        rlflag(dirName+"/"+ direc.at(d));
    } 
    
    if((closedir(dirp)) == -1)
    {
        perror("Error with closedir: ");
    }
    
    return 0;
}


int main(int argc, char *argv[])
{
    vector <char*> v; //this will contain everything the user inputs
    bool lflags = false, aflags = false, rflags = false;
    char* input; //the char and string will help keep track of files/directories
    string cinput = "";
    vector <string> in; //this will contain all the files/directories the user inputs

    for(int i = 1; i < argc; i++) //gathers user input into a vector, starts at 1 so it does not include ./ls 
    {
        v.push_back(argv[i]);
    }

    int vsize = v.size(); //size of vector
    
    for(int d = 0; d < vsize; d++) //time to check every element in the vector
    {
        if(strcmp(v.at(d), "-l") == 0) //if user has enetered -l
        {
            lflags = true;
        }

        else if(strcmp(v.at(d), "-a") == 0) //-a
        {
            aflags = true;
        }

        else if(strcmp(v.at(d), "-R") == 0) //-R
        {
            rflags = true;
        }
        
        else if((strcmp(v.at(d), "-la") == 0) || (strcmp(v.at(d), "-al") == 0))
        {
            lflags = true;
            aflags = true;
        }
        else if((strcmp(v.at(d), "-aR") == 0) || (strcmp(v.at(d), "-Ra") == 0))
        {
            rflags = true;
            aflags = true;
        }       
        else if((strcmp(v.at(d), "-lR") == 0) || (strcmp(v.at(d), "-Rl") == 0))
        {
            rflags = true;
            lflags = true;
        }

        
        else if((strcmp(v.at(d), "-alR") == 0) || (strcmp(v.at(d), "-aRl") == 0))
        {
            aflags = true;
            lflags = true;
            rflags = true;
        }
        else if((strcmp(v.at(d), "-laR") == 0) || (strcmp(v.at(d), "-lRa") == 0))
        {
            rflags = true;
            aflags = true;
            lflags = true;
        }
        else if((strcmp(v.at(d), "-Ral") == 0) || (strcmp(v.at(d), "-Rla") == 0))
        {
            rflags = true;
            aflags = true;
            lflags = true;
        }
        //this is if there is something in the input that isn't any of the flags. this means it is either a file/directory
        else //or invalid input
        {
           input = v.at(d);
           cinput = string(input);
           in.push_back(cinput);
        }
    }
   
    int isize = in.size();
    //previous part was checking for flags & file/directory, now we call the functions based on the user input

    //no flags
    if(!lflags && !aflags && !rflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                cout << in.at(i) << ":"<< endl;
                noflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            noflag(cinput);
        }
    }

    //-l flag only
    if(lflags && !aflags && !rflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                cout << in.at(i) << ":"<< endl;
                lflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            lflag(cinput);
        }
        
    }

    //-rflag only
    if(rflags && !aflags && !lflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                rflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            rflag(cinput);
        }
       
    }

    //-a flag only
    if(aflags && !lflags && !rflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                cout << in.at(i) << ":"<< endl;
                aflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            aflag(cinput);
        }
    }

    //-l and -a flag only
    if(lflags && aflags && !rflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                cout << in.at(i) << ":"<< endl;
                laflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            laflag(cinput);
        }
    }
    
    //-a and -r flag only
    if(aflags && rflags && !lflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                raflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            raflag(cinput);
        }
    }

    //-r and -l flag
    if(rflags && lflags && !aflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                rlflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            rlflag(cinput);
        }
    }

    //all three flags
    if(rflags && lflags && aflags)
    {
        if(isize>1)//if there is more than one file or directory
        {
            for(int i = 0; i < isize; i++)
            {
                rlaflag(in.at(i));
                cout << endl;
            }
        }

        else
        {
            rlaflag(cinput);
        }
    }

    return 0;
}






