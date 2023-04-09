#include "fsLow.h"
#include "vcb.h"
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
    return freeSpaceManagerBlocks + 1;
}

// int findFreeBlocks(int blockCount)
// {

//     // int *freeSpaceManager = malloc(5 * 512 * sizeof(int));
//     // LBAread(freeSpaceManager, 5, 1);

//     // for (int i = 0; i < 2560; i++)
//     // {
//     //     if (freeSpaceManager[i] == 0)
//     //     {
//     //         for (int j = i + 1; j < blockCount; j++)
//     //         {
//     //             if (freeSpaceManager[j] == 1)
//     //             {
//     //                 break;
//     //             }
//     //             else
//     //             {
//     //                 return i;
//     //             }
//     //         }
//     //     }
//     // }

//     free(freeSpaceManager);
//     return 0;
// }