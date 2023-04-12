/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
 * GitHub Name: copbrick
 * Group Name: Team DALJ
 * Project: Basic File System
 *
 * File: mfs.c
 *
 * Description: to be done
 *
 * 
 *
 **************************************************************/
#include "mfs.h"
char patharray[52][256];
void parsePath(char*path){
    char pathname[256];
    strcpy(pathname,path);
    char* token;
    char* rest;
    int i=0;
    token = strtok_r(pathname,"/",&rest);
    while(token!=NULL){
        strcpy(patharray[i],token);
        i++;
        token = strtok_r(NULL,"/",&rest);
    }
    
}

int main() {
    char* path = "home/documents/student/file";
    parsePath(path);
    for(int i = 0; i < 52; i++) {
        for(int j = 0; j < 256; j++) {
            printf("%c", patharray[i][j]);
        }
        printf("\n");
    }

    return 0;
}
