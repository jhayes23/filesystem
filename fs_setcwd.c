#include <stdio.h>
#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
#include "directoryEntry.h"
#include "fsLow.h"
#include "vcb.h"
#include "parsePath.h"

int fs_setcwd(char *pathname)
{
    parsedPath parsed;
    parsed = parsePath(pathname);
    if (parsed.index > -1)
    {
        strcpy(currentWorkDir, parsed.path);
        return 0;
    }
    return -1;
}