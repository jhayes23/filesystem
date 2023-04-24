#include "mfs.h"
#include "parsePath.h"
#include <stdio.h>

int fs_isFile(char *fileName)
{

    parsedPath path = parsePath(fileName, NULL);
    int test = 1;
    printf("Parent Dir Entry Index: %d\n", test);
    return 0;
}