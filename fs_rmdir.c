/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: fs_rmdir.c
 *
 * Description: Deletes a directory at specified path
 *
 *
 *
 **************************************************************/
#include <stdio.h>
#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
#include "directoryEntry.h"
#include "fsLow.h"
#include "vcb.h"
#include "parsePath.h"
#include "freeSpaceManager.h"

int fs_rmdir(const char *pathname)
{
    int bytesNeed = initDirAmt * sizeof(directoryEntry);
    int blkCount = (bytesNeed + vcb->blockSize - 1) / vcb->blockSize;
    int byteUsed = blkCount * vcb->blockSize;
    parsedPath parsed = parsePath(pathname);

    if (parsed.index > 0)
    { // If path is reachable
        // load dir into memory
        directoryEntry *checkDir = malloc(byteUsed);
        LBAread(checkDir, blkCount, parsed.parent[parsed.index].location);

        // checks the directory for any files/directories before attempting to delete
        int directoryEmpty = directoryIsEmpty(checkDir);
        if (directoryEmpty == 0)
        {
            parsed.parent[parsed.index].location = 0;
            parsed.parent[parsed.index].fileSize = 0;
            strcpy(parsed.parent[parsed.index].fileName, "\0");

            LBAwrite(parsed.parent, blkCount, parsed.parent[0].location);
            // TODO set freespace blocks to available
        }
        // free memory allocations
        free(checkDir);
        free(parsed.parent);
        parsed.parent = NULL;
        checkDir = NULL;
    }
}