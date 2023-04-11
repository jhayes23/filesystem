// #include "vcb.h"
#include "fsLow.h"
#include "stdlib.h"

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
    VCB *vcb = malloc(vcb->blockSize);
    LBAread(vcb, 1, 0);
    printf("VCB->signature: %d\n", vcb->signature);
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
    free(vcb);
    free(freeSpaceManager);
    return -1;
}