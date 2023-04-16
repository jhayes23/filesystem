#include <sys/types.h>
#include "vcb.h"
#include "fsLow.h"
#include "stdlib.h"
#include "freeSpaceManager.h"
#include "stdio.h"
#include "string.h"

/**
 * initFreeSpaceManager initalizes our bitmap array. We reserve space
 * in memory for the manager, then we fill the first blocks that the manager takes up
 * as occupied (1). We then fill up the rest of our bitmap array with 0's to indicate free blocks.
 *
 * We then write to the disk and return the starting position of the freeSpaceManager
 */
int initFreeSpaceManager(int totalBlocks, int blockSize)
{
    // // int freeSpaceManagerBlocks = totalBlocks / (8 * blockSize) + 1;

    // int *freeSpaceManager = malloc((freeSpaceManagerBlocks * blockSize) * sizeof(int));
    // for (int i = 0; i < freeSpaceManagerBlocks; i++)
    // {
    //     freeSpaceManager[i] = 1;
    // }
    // for (int j = freeSpaceManagerBlocks; j <= freeSpaceManagerBlocks * blockSize; j++)
    // {
    //     freeSpaceManager[j] = 0;
    // }
    // LBAwrite(freeSpaceManager, freeSpaceManagerBlocks, 1);
    // return 1;

    // bytes required for our bitmap
    int bytesNeeded = (totalBlocks / 8) + 1;

    // blocks required for our bitmap
    int freeSpaceManagerBlocks = (bytesNeeded / blockSize) + 1;

    // initializing our bitmap
    unsigned char *bitMap = (unsigned char *)malloc(bytesNeeded);

    // set all the bits to zero
    memset(bitMap, 0, bytesNeeded);

    // mark first 5 blocks is used.
    for (int i = 0; i < freeSpaceManagerBlocks; i++)
    {
        setBit(bitMap, i);
    }

    // write back to disk
    LBAwrite(bitMap, freeSpaceManagerBlocks, 1);

    // return the starting block of free space
    return 1;
}

void setBit(unsigned char *bitMap, int blockNumber)
{
    // get the byte index;
    int byteIndex = blockNumber / 8;

    int bitIndex = blockNumber % 8;

    // set 1 if not zero
    bitMap[byteIndex] = bitMap[byteIndex] | 1 << bitIndex;
}

/**
 * This function allows the file system to request N amount of blocks,
 * then we check the first group of blocks that are 0 and equals requestedBlocks.
 * Once we find this group, we return the starting index of the blocks.
 */
int findFreeBlocks(int requestedBlocks)
{
    int *freeSpaceManager = malloc(5 * vcb->blockSize * sizeof(int));

    int freeSpaceManagerSize = 5 * vcb->blockSize * sizeof(int);
    LBAread(freeSpaceManager, 5, 1);

    for (int i = 0; i < freeSpaceManagerSize - requestedBlocks + 1; i++)
    {
        int freeBlockCount = 0;
        for (int j = 0; j < requestedBlocks; j++)
        {
            if (freeSpaceManager[i + j] == 0)
            {
                freeBlockCount++;
            }
            else
            {
                break;
            }
        }
        if (freeBlockCount == requestedBlocks)
        {
            return i;
        }
    }
    // free(vcb);
    // free(freeSpaceManager);
    return -1;
}