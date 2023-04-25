#include "mfs.h"
#include <stdio.h>
#include "directoryEntry.h"
#include "parsePath.h"

int fs_isFile(char *fileName)
{

    parsedPath path = parsePath(fileName);
    int test = 1;
    printf("Parent Dir Entry Index: %d\n", test);
    return 0;
}