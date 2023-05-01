#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "directoryEntry.h"
#include "mfs.h"
#include "parsePath.h"
#include "vcb.h"
#include "fsLow.h"

struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{
    if (dirp == NULL)
    {
        printf("read error: can't found directory\n");
        return NULL;
    }
    int blocks = (dirp->DE->fileSize + vcb->blockSize - 1) / vcb->blockSize;
    LBAread(dirp->DE, blocks, dirp->directoryStartLocation);
    dirp->dirEntryPosition++;
    if (dirp->DE[dirp->dirEntryPosition].isFile == DIRECTORY)
    {
        dirp->dirinfo->fileType = FT_DIRECTORY;
    }
    else
    {
        dirp->dirinfo->fileType = FT_REGFILE;
    }
    dirp->dirinfo->d_reclen = sizeof(dirp->dirinfo);
    strcpy(dirp->dirinfo->d_name, dirp->DE[dirp->dirEntryPosition].fileName);
    return dirp->dirinfo;
}