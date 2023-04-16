#ifndef FREESPACEMANAGER_H
#define FREESPACEMANAGER_H

int initFreeSpaceManager(int totalBlocks, int blockSize);
int findFreeBlocks(int requestedBlocks);
void setBit(unsigned char *freeSpaceManager, int blockNumber,int isFree);
int checkBit(unsigned char * freeSpaceManager,int bitIndex);
void freeBlocks(unsigned char * freeSpaceManger,int startIndex,int numberOfBlocks);

void printBitMap(unsigned char * freeSpaceManager);

#endif