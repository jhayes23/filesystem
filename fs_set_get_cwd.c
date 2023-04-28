#include <stdio.h>
#include <unistd.h>
#include "parsePath.h"


int fs_setcwd(char *pathname)
{

    // pathname has to exist
    // the last element has to be a direcotry

    parsePath path = parsePath(pathname);

    if(path.parent != NULL)
    {

        int result = chdir(pathname);

        // 0 --> current working directory is changed successfully
        if(result == 0) 
        {
            printf("Success changing CWD. \n");
        }
        else
        {
            printf("Error changing CWD. \n");
            return -1;
        }
    }

    return 0;
}


char *fs_getcwd(char *pathname,size_t size) 
{
    // copy of pathname
    char * cwd = (char *)malloc(size * sizeof(char));

    // copying the pathname until reaching to null termiantor
    strncpy(cwd,pathname, size - 1);
    // setting the last element to null terminator
    cwd[size - 1] = '\0';

    if(getcwd(cwd,sizeof(cwd)) == NULL)
    {
        printf("Error getting current working directory \n");
        return -1;
    }

    printf("Current working directory : %s \n", cwd);

    return cwd;
}