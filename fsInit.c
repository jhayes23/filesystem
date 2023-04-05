/**************************************************************
 * Class:  CSC-415-0# Fall 2021
 * Names:
 * Student IDs:
 * GitHub Name:
 * Group Name:
 * Project: Basic File System
 *
 * File: fsInit.c
 *
 * Description: Main driver for file system assignment.
 *
 * This file is where you will start and initialize your system
 *
 **************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"

#define magicNumber 734743912 //random signature
typedef struct VCB {
	int signature; // signature to know if filesystem has been initialized
	int blockSize; //size of each block
	int totalBlocks; //total blocks available
	int freeBlocks; //number of free blocks
	int freeSpaceBlock; //starting block for free space manager
	int rootDirBlock; //starting block for root directory
} VCB;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	void *VCB = malloc(blockSize);
	char *buffer;
	int blocksRead = LBAread(buffer, 0, numberOfBlocks);
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}