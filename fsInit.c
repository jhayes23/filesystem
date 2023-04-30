/**************************************************************
 * Class:  CSC-415-01 Spring 2023
 * Names: Anthony Benjamin, Nyan Ye Lin, Joshua Hayes, David Chen
 * Student IDs: 921119898, 921572181, 922379312, 922894099
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
#include "freeSpaceManager.h"
#define magicNumber 734743927 // random signature

VCB *vcb; // global declaration of VCB

int initRootDir(int blockSize)
{
	// 52 directory entries * 136 sizeof 1 directory entry = 7072 bytes/ 512 chunks = 14 blocks

	directoryEntry *dir = (directoryEntry *)malloc(initDirAmt);
	dir = initDir(initDirAmt, NULL);
	int rootposition = dir[0].location;

	free(dir);
	dir = NULL;

	return rootposition;
}

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	vcb = malloc(blockSize);
	int blocksRead = LBAread(vcb, 1, 0);
	printf("blocksRead: %d\n", blocksRead);

	if (LBAread(vcb, 1, 0) == 1) // Read block 0 from disk into VCB
	{
		// // init vcb since signature is missing or does not match
		// vcb->signature = magicNumber;
		// vcb->blockSize = blockSize;
		// vcb->totalBlocks = numberOfBlocks;
		// vcb->freeBlocks = numberOfBlocks - 6;

		// // Set vals returned from init procedures

		// vcb->freeSpaceManagerBlock = initFreeSpaceManager(vcb->totalBlocks, vcb->blockSize);
		// printf("freeSpaceManBlock: %d\n", vcb->freeSpaceManagerBlock);

		// vcb->rootDirBlock = initRootDir(vcb->blockSize);
		// printf("Start of RootDir: %d\n",vcb->rootDirBlock);

		// // LBAwrite VCB back to disk
		// LBAwrite(vcb, 1, 0);

		if (vcb->signature != magicNumber) // System not initalized
		{
			// init vcb since signature is missing or does not match
			vcb->signature = magicNumber;
			vcb->blockSize = blockSize;
			vcb->totalBlocks = numberOfBlocks;
			vcb->freeBlocks = numberOfBlocks;

			// Set vals returned from init procedures

			// struct fs_stat *test;
			// int result = fs_stat("~",test);
			// printf("RESULT : %d\n",result);




			vcb->freeSpaceManagerBlock = initFreeSpaceManager(vcb->totalBlocks, vcb->blockSize);
			vcb->rootDirBlock = initRootDir(vcb->blockSize);

			LBAwrite(vcb, 1, 0); // LBAwrite VCB back to disk
		}
		else
		{
			// loadFreeSpace into memory
			printf("VCB ALREADY INITALIZED;\nSignature: %d\n", vcb->signature);
		}
	}

	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}