/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312
 * GitHub Name: copbrick
 * Group Name: Team DALJ
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
#include "vcb.h"
#include "directoryEntry.h"
#define initDirAmt 52
#define magicNumber 734743917 // random signature

/**
 * initFreeSpaceManager initalizes our bitmap array. We reserve space
 * in memory for the manager, then we fill the first blocks that the manager takes up
 * as occupied (1). We then fill up the rest of our bitmap array with 0's to indicate free blocks.
 * 
 * We then write to the disk and return the starting position of the freeSpaceManager
*/
int initFreeSpaceManager(int totalBlocks, int blockSize)
{
	int freeSpaceManagerBlocks = totalBlocks / (8 * blockSize) + 1;
	int *freeSpaceManager = malloc((freeSpaceManagerBlocks * blockSize) * sizeof(int));
	for (int i = 0; i < freeSpaceManagerBlocks; i++)
	{
		freeSpaceManager[i] = 1;
	}
	for (int j = freeSpaceManagerBlocks; j <= freeSpaceManagerBlocks * blockSize; j++)
	{
		freeSpaceManager[j] = 0;
	}
	LBAwrite(freeSpaceManager, freeSpaceManagerBlocks, 1);
	return 1;
}
/**
 * This function allows the file system to request N amount of blocks,
 * then we check the first group of blocks that are 0 and equals requestedBlocks.
 * Once we find this group, we return the starting index of the blocks.
*/
int findFreeBlocks(int requestedBlocks)
{
	int freeBlockCount = 0;
	int startIndex = -1;

	VCB *vcb = malloc(512);
	LBAread(vcb, 1, 0);
	int *freeSpaceManager = malloc(5 * 512 * sizeof(int));

	int freeSpaceManagerSize = 5 * 512 * sizeof(int);

	LBAread(freeSpaceManager, 5, 1);

	printf("VCB->totalBlocks: %d\n", vcb->totalBlocks);
	
	for (int i = 0; i < freeSpaceManagerSize - requestedBlocks; i++)
	{
		if (freeSpaceManager[i] == 0)
		{
			freeBlockCount += 1;
			startIndex = i;

			for (int j = i + 1; j < requestedBlocks - 1; j++)
			{

				if (freeSpaceManager[j] == 1)
				{
					// reset
					startIndex = -1;
					freeBlockCount = 0;
					break;
				}
				freeBlockCount++;
			}
			if (freeBlockCount == requestedBlocks)
			{
				return startIndex;
			}
		}

	}
	return -1;
}

int initRootDir(int blockSize)
{
	// 52 directory entries * 136 sizeof 1 directory entry = 7072 bytes/ 512 chunks = 14 blocks
	struct directoryEntry *directory = malloc(initDirAmt * sizeof(directoryEntry));
	// for (int i = 0; i < initDirAmt; i++)
	// {
	// 	// mark entry as free
	// }

	// Todo: Ask free space for 14 blocks:
	// free space returns start location of the 14 blocks (i.e: 6)

	// ask for 14 blocks here
	int firstFreeBlock = findFreeBlocks(14);

	// printf("FIRST FREE BLOCK %d \n", firstFreeBlock);

	strncpy(directory[0].fileName, ".", 1);
	strncpy(directory[1].fileName, "..", 2);
	LBAwrite(directory, 14, 6); // LBA write 14 blocks starting at 6

	return firstFreeBlock; // return start location
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

			VCB->freeSpaceManagerBlock = initFreeSpaceManager(VCB->totalBlocks, VCB->blockSize);
			VCB->rootDirBlock = initRootDir(VCB->blockSize);

			LBAwrite(VCB, 1, 0); // LBAwrite VCB back to disk
		}
		else
		{
			// loadFreeSpace into memory
			printf("VCB ALREADY INITALIZED;\nSignature: %d\n", VCB->signature);
			printf("START OF FREE BLOCK %d\n", VCB->rootDirBlock);
			printf("VCB POINTER : %p\n", VCB);
		}
	}
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}