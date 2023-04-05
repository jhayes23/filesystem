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
#define initDirAmt 52
#define magicNumber 734743912 // random signature

typedef struct directoryEntry
{
	// size of struct is 136 bytes
	int id;
	int isFile;
	int location;
	int fileSize;
	time_t createDate;
	time_t lastAccessDate;
	time_t lastModifyDate;
	char fileName[64];
	char author[32];
} directoryEntry;
typedef struct VCB
{
	int signature;		// signature to know if filesystem has been initialized
	int blockSize;		// size of each block
	int totalBlocks;	// total blocks available
	int freeBlocks;		// number of free blocks
	int freeSpaceBlock; // starting block for free space manager
	int rootDirBlock;	// starting block for root directory
} VCB;

int initRootDir(int blockSize)
{
	// 52 directory entries * 136 sizeof 1 directory entry = 7072 bytes/ 512 chunks = 14 blocks
	struct directoryEntry *directory = malloc(initDirAmt * sizeof(directoryEntry));
	for (int i = 0; i < initDirAmt; i++)
	{
		// mark entry as free
	}

	// Todo: Ask free space for 14 blocks:
	// free space returns start location of the 14 blocks (i.e: 6)

	strncpy(directory[0].fileName, ".", 1);
	strncpy(directory[1].fileName, "..", 2);
	LBAwrite(directory, 14, 6); // LBA write 14 blocks starting at 6

	return 6; //return start location
}

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	struct VCB *VCB = malloc(blockSize);
	if (LBAread(VCB, 1, 0) == 1) // Read block 0 from disk into VCB
	{
		if (VCB->signature != magicNumber) // System not initalized
		{
			// init vcb since signature is missing or does not match
			VCB->signature = magicNumber;
			VCB->blockSize = blockSize;
			VCB->totalBlocks = numberOfBlocks;
			VCB->freeBlocks = numberOfBlocks;

			// Set vals returned from init procedures

			// vcb->freeSpaceBlock = initFreeSpace(numberOfBlocks, blockSize);
			VCB->rootDirBlock = initRootDir(blockSize);

			LBAwrite(VCB, 1, 0); // LBAwrite VCB back to disk
		}
		else
		{
			// loadFreeSpace into memory
			printf("VCB ALREADY INITALIZED;\nSignature: %d\n", VCB->signature);
		}
	}
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}