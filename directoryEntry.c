#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "vcb.h"
#include "fsLow.h"
#include "directoryEntry.h"
#include "freeSpaceManager.h"

directoryEntry * rootDir;
char currentWorkDir[]; 

directoryEntry *initDir(int minNumEntries, directoryEntry *parent)
{
	int bytesNeed = minNumEntries * sizeof(directoryEntry);
	int blkCount = (bytesNeed + vcb->blockSize - 1) / vcb->blockSize;
	int byteUsed = blkCount * vcb->blockSize;
	int trueNumEntries = byteUsed / sizeof(directoryEntry);
	bytesNeed = trueNumEntries * sizeof(directoryEntry);

	directoryEntry *dir = malloc(byteUsed);

	int startBlock = findFreeBlocks(blkCount);
	printf("startblock: %d\n", startBlock);

	for (int i = 2; i < trueNumEntries; i++)
	{
		strcpy(dir[i].fileName, "\0");
	}

	time_t t = time(NULL);
	strcpy(dir[0].fileName, ".");
	dir[0].createDate = t;
	dir[0].lastModifyDate = t;
	dir[0].lastAccessDate = t;
	dir[0].location = startBlock;
	dir[0].fileSize = bytesNeed;
	dir[0].isFile = DIR;
	strcpy(dir[1].fileName, "..");
	if (parent ==  NULL)
	{
		dir[1].createDate = dir[0].createDate;
		dir[1].lastModifyDate = dir[0].lastModifyDate;
		dir[1].lastAccessDate = dir[0].lastAccessDate;
		dir[1].location = dir[0].location;
		dir[1].fileSize = dir[0].fileSize;
		dir[1].isFile = dir[0].isFile;
	}
	else
	{
		dir[1].createDate = parent->createDate;
		dir[1].lastModifyDate = parent->lastModifyDate;
		dir[1].lastAccessDate = parent->lastAccessDate;
		dir[1].location = parent->location;
		dir[1].fileSize = parent->fileSize;
		dir[1].isFile = parent->isFile;
	}
	LBAwrite(dir, blkCount, startBlock);
	return dir;
}