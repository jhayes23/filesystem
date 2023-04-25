/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: parsePath.c
 *
 * Description: Path parser and utility procedures
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

void loadDirectory(directoryEntry *loadDir, int dirStartBlock) //Loads Directory into memory
{
    int bytesNeed = initDirAmt * sizeof(directoryEntry);
    int blkCount = (bytesNeed + vcb->blockSize - 1) / vcb->blockSize;
    LBAread(loadDir, blkCount, dirStartBlock);
}

int locateEntry(directoryEntry *dir, char *entry) //Searches parent directory for matching entry
{ // search directory, entry = name
    for (int i = 0; i < (dir->fileSize) / sizeof(directoryEntry); i++)
    {
        if (strcmp(dir[i].fileName, entry) == 0)
        {
            return i;
        }
    }
    return -1;
}

parsedPath parsePath(const char *path) //Parses a path and returns a struct containing
//a pointer to the parent directory, the index of entry, and the absolute path
{
    char pathname[256];
    strcpy(pathname, path);
    int bytesNeed = initDirAmt * sizeof(directoryEntry);
    int blkCount = (bytesNeed + vcb->blockSize - 1) / vcb->blockSize;
    int byteUsed = blkCount * vcb->blockSize;
    directoryEntry *dirToParse = malloc(byteUsed);
    parsedPath pathRet;
    pathRet.path = malloc(256);

    if (path == NULL)
    {
        pathRet.index = 0;
        pathRet.parent = NULL;
        strcpy(pathRet.path, "\0");
        strcpy(pathRet.dirName, "\0");
        return pathRet;
    }
    else if(strcmp(path,"~") == 0 ){
        LBAread(dirToParse, blkCount, vcb->rootDirBlock);
        pathRet.index = 0;
        pathRet.path = "/";
        pathRet.parent = dirToParse;
        return pathRet;
    }
    else if (path[0] != '/')
    {
        strcpy(pathname, currentWorkDir);
        int lastChar = (int)strlen(pathname) - 1;
        if (pathname[lastChar] != '/')
        {
            strcat(pathname, "/");
        }
        strcat(pathname, path);
        strcpy(pathRet.path, pathname);
    }
    LBAread(dirToParse, blkCount, vcb->rootDirBlock);
    char *token;
    char *endingToken = NULL;
    char *rest;
    token = strtok_r(pathname, "/", &rest);
    if (token != NULL)
    {
        endingToken = strtok_r(NULL, "/", &rest);
    }
    int entryIndex = -1;
    while (endingToken != NULL)
    {
        entryIndex = locateEntry(dirToParse, token);
        token = endingToken;
        endingToken = strtok_r(NULL, "/", &rest);

        if (entryIndex == -1 && (dirToParse[entryIndex].isFile == FILEMACRO && endingToken != NULL))
        {
            // Cannot continue since entry is a file and more to process
            pathRet.index = entryIndex;
            pathRet.parent = NULL;
            return pathRet;
        }

        loadDirectory(dirToParse, dirToParse[entryIndex].location);
    }
    pathRet.index = locateEntry(dirToParse, token);
    pathRet.parent = dirToParse;
    strcpy(pathRet.dirName, token);

    return pathRet;
}

int findOpenEntrySlot(directoryEntry *parent) //Searches parent dir for open dir entry
{
    for (int i = 0; i < (parent->fileSize) / sizeof(directoryEntry); i++)
    {
        if (strcmp(parent[i].fileName, "\0") == 0)
        {
            return i;
        }
    }
    printf("No available entries.\n");
    return -1;
}

int directoryIsEmpty(directoryEntry *parent) // Checks if parent directory has any sub entries
{
    for (int i = 2; i < (parent->fileSize) / sizeof(directoryEntry); i++)
    {
        if (strcmp(parent[i].fileName, "\0") != 0)
        {
            return -1;
        }
    }
    return 0;
}