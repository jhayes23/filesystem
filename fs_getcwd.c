#include <stdio.h>
#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
#include "directoryEntry.h"
#include "fsLow.h"
#include "vcb.h"
#include "parsePath.h"

char *fs_getcwd(char *pathname, size_t size)
{
    if (strlen(currentWorkDir) > size)
    {
        return NULL;
    }
    strcpy(pathname, currentWorkDir);
    return currentWorkDir;
}