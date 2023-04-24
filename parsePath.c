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
 * Description:
 *
 *
 *
 **************************************************************/
// /home/student/Documents/myfile

#include <stdio.h>
#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
#include "directoryEntry.h"
#include "fsLow.h"
#include "vcb.h"
#include "parsePath.h"

void loadDirectory(directoryEntry *loadDir, directoryEntry *dir)
{
    LBAread(loadDir, dir->fileSize / vcb->blockSize, dir->location);
}

int locateEntry(directoryEntry *dir, char *entry)
{ // search directory, entry = name
    for (int i = 0; i < (dir->fileSize) / sizeof(directoryEntry); i++)
    {
        printf("I: %d   EntryName: [%s]\n", i, dir[i].fileName);
        if (strcmp(dir[i].fileName, entry) == 0)
        {
            printf("FOUND: %s\n", dir[i].fileName);
            return i;
        }
    }
    return -1;
}

parsedPath parsePath(char *path, directoryEntry *startPath)
{

    char pathname[256];
    strcpy(pathname, path);

    printf("Path to parse: %s\n", pathname);
    directoryEntry *dir = startPath;
    if (startPath == NULL)
    {
        dir = vcb->rootDirBlock;
    }
    else
    {
        dir = startPath;
    }

    char *token;
    char *rest;
    token = strtok_r(pathname, "/", &rest);
    int entryIndex = -1;
    parsedPath pathRet;

    while (token != NULL)
    {
        printf("Token: %s\n", token);
        printf("test");
        entryIndex = locateEntry(dir, token);
        token = strtok_r(NULL, "/", &rest);
        if (entryIndex == -1 || (dir[entryIndex].isFile == FILE && token != NULL))
        {

            pathRet.index = entryIndex;
            pathRet.parent = NULL;
            return pathRet;
        }
        loadDirectory(dir, dir + entryIndex);
    }

    pathRet.index = entryIndex;
    pathRet.parent = dir;
    return pathRet;
}