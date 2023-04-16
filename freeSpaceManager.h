#ifndef FREESPACEMANAGER_H
#define FREESPACEMANAGER_H

int initFreeSpaceManager(int totalBlocks, int blockSize);
int findFreeBlocks(int requestedBlocks);
void setBit(unsigned char *bitMap, int blockNumber,int isFree);
int checkBit(int bitIndex);

#endif