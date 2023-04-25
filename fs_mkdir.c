/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: mkdir.c
 *
 * Description: Creates a directory at specified path
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

int fs_mkdir(const char *pathname, mode_t mode)
{
    parsedPath parsed = parsePath(pathname);
    if (parsed.parent != NULL && parsed.index == -1)
    {
        // Searches parent directory for available slot
        int entryIndex = findOpenEntrySlot(parsed.parent);

        if (entryIndex > 0)
        {
            // Create directory
            directoryEntry *createDir = initDir(initDirAmt, parsed.parent);
            // copy directory info into parent
            strcpy(parsed.parent[entryIndex].fileName, parsed.dirName);
            parsed.parent[entryIndex].location = createDir->location;
            parsed.parent[entryIndex].lastModifyDate = createDir->lastModifyDate;

            // writes back to disk
            int bytesNeed = initDirAmt * sizeof(directoryEntry);
            int blkCount = (bytesNeed + vcb->blockSize - 1) / vcb->blockSize;
            LBAwrite(parsed.parent, blkCount, parsed.parent->location);

            // free memory allocations
            free(parsed.parent);
            parsed.parent = NULL;
            return 0;
        }
    }
    // Unable to reach directory or
    printf("Unable to create directory %s\n", pathname);
    return -1;
}