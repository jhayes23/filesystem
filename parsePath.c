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
//LOAD ROOT DIR
//LOOK FOR ENTRY WHOSE NAME IS HOME
//LOAD DATA BLOB that is the home directory (LOAD Directory function)
//LOOK FOR student
//LOAD student  if student exist and is directory
//LOOK for Documents
//LOAD Documents
//LOOK FOR myfile

//yes valid path
//last component  -> exist/doesn't exist -> if exists its a dir or file

//return whether or not its a valid path ( primary components are not valid). if it exists which directory entry index is it
// ptr * to loaded directory
//if not exists, index = -1
//returns parent directory of the index
#include <stdio.h>
#include "mfs.h"
#include <stdlib.h>
#include <unistd.h>
#include "directoryEntry.h"
#include "fsLow.h"


void parsePath(char * path){

    char pathname[256];
    strcpy(pathname,path);

    directoryEntry * dir;
    char* token;
    char* rest;
    token = strtok_r(pathname,"/",&rest);

    while(token != NULL){
        printf("Token: %s\n",token);

        int index = locateEntry(dir,token);
        if(index == -1 || dir[index].isFile == 0){
            printf("Does not exist or is file.\n");
            //return
        }
        loadDirectory(dir,dir[index]);
        token = strtok_r(NULL,"/",&rest);
    }
    //do return
}

void loadDirectory(directoryEntry * loadDir, directoryEntry dir){
    LBAread(loadDir,dir.fileSize/512,dir.location); //TODO find blockCount after git pull
}

int locateEntry( directoryEntry * dir, char * entry){ // search directory, entry = name
    for (int i = 0; i < (dir->fileSize)/sizeof(directoryEntry); i++)//TODO hopefully num of entries
    {
        printf("I: %d   EntryName: [%s]\n",i,dir->fileName);
        if(strcmp(dir[i].fileName,entry) == 0)
        return i;
    }
    return -1;
}

int main() {
    // char* path = "home/documents/student/file";
    // parsePath(path);
    // for(int i = 0; i < 52; i++) {
    //     for(int j = 0; j < 256; j++) {
    //         printf("%c", patharray[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}