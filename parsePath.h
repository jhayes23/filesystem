/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: parsePath.h
 *
 * Description: Path parser and utility procedures header
 *
 * 
 *
 **************************************************************/
#include "directoryEntry.h"
typedef struct
{
    int index;
    char dirName[64];
    char childName[64];
    char  * path;
    directoryEntry * parent; 
}parsedPath;

void loadDirectory(directoryEntry * loadDir,int dirStartBlock);
int locateEntry( directoryEntry * dir, char * entry);
parsedPath parsePath(const char * path);
int findOpenEntrySlot(directoryEntry * parent);
int directoryIsEmpty(directoryEntry * parent);
char * resolvePath(const char *path);