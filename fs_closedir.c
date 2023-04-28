#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "directoryEntry.h"
#include "mfs.h"
#include "parsePath.h"
#include "vcb.h"
#include "fsLow.h"
int fs_closedir(fdDir *dirp)
{
    if (dirp == NULL)
    {
        printf("close error: no such directory\n");
        return -1;
    }
    dirp->lastAccessDate = time(NULL);
    dirp->dirEntryPosition = 0;
    dirp->dirinfo = NULL;
    dirp->directoryStartLocation = 0;
    free(dirp->DE);
    free(dirp->dirinfo);
    free(dirp);
    return 0;
}
