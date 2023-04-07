typedef struct VCB
{
    int signature;             // signature to know if filesystem has been initialized
    int blockSize;             // size of each block
    int totalBlocks;           // total blocks available
    int freeBlocks;            // number of free blocks
    int freeSpaceManagerBlock; // starting block for free space manager
    int rootDirBlock;          // starting block for root directory
} VCB;