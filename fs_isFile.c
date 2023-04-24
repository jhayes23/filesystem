#include "mfs.h"
#include "parsePath.h"

int fs_isFile(char *fileName)
{

    parsedPath path = parsePath(fileName, NULL);
    printf("path: %zu", path);
    return 0;
}