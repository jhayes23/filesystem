/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: openreadclosedir.c
 *
 * Description: open directory, read directory and close directory
 *
 *
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "directoryEntry.h"
#include "mfs.h"
#include "fsLow.h"
#include "vcb.h"
#include "parsePath.h"
fdDir *fs_opendir(const char *pathname)
{
    fdDir *dir = malloc(sizeof(fdDir));
    parsedPath parsed = parsePath(pathname);
    if (parsed.index == -1)
    {
        printf("open error: can't found such directory\n");
        return NULL;
    }
    if (parsed.parent[parsed.index].isFile != DIRECTORY)
    {
        printf("open error: this is not a directory\n");
        fs_closedir(dir);
        return NULL;
    }
    dir->lastAccessDate = time(NULL);
    dir->directoryStartLocation = parsed.parent[parsed.index].location;
    dir->dirEntryPosition = 0;
    dir->DE = malloc(parsed.parent[parsed.index].fileSize);
    dir->DE->fileSize = parsed.parent[parsed.index].fileSize;
    strcpy(dir->dirinfo->d_name, parsed.parent[parsed.index].fileName);
    dir->d_reclen = sizeof(fdDir);
    return dir;
}
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
