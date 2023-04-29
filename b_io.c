/**************************************************************
 * Class:  CSC-415-0# Fall 2021
 * Names:
 * Student IDs:
 * GitHub Name:
 * Group Name:
 * Project: Basic File System
 *
 * File: b_io.c
 *
 * Description: Basic File System - Key File I/O Operations
 *
 **************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "fsLow.h"
#include "b_io.h"
#include "vcb.h"
#include "parsePath.h"
#include "freeSpaceManager.h"
#include "directoryEntry.h"
#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
{
	/** TODO add al the information you need in the file control block **/
	char *buf;		// holds the open file buffer
	int index;		// holds the current position in the buffer
	int buflen;		// holds how many valid bytes are in the buffer
	int currentBlk; // holds the current bloc number
	int numBlocks;	// holds how many blocks file occupies
	int fileSize;
	int fileOffset; // holds the current position that the file is at
	int flags;
} b_fcb;

b_fcb fcbArray[MAXFCBS];

int startup = 0; // Indicates that this has not been initialized

// Method to initialize our file system
void b_init()
{
	// init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
	{
		fcbArray[i].buf = NULL; // indicates a free fcbArray
	}

	startup = 1;
}

// Method to get a free FCB element
b_io_fd b_getFCB()
{
	for (int i = 0; i < MAXFCBS; i++)
	{
		if (fcbArray[i].buf == NULL)
		{
			return i; // Not thread safe (But do not worry about it for this assignment)
		}
	}
	return (-1); // all in use
}

// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open(char *filename, int flags)
{
	b_io_fd returnFd;
	//*** TODO ***:  Modify to save or set any information needed
	//
	//

	if (startup == 0)
		b_init(); // Initialize our system

	returnFd = b_getFCB(); // get our own file descriptor
						   // check for error - all used FCB's
	if (returnFd < 0)
	{
		return -1;
	}
	parsedPath parsed = parsePath(filename);
	int pos=0;
	//if file exists
	if(parsed.index==-1){
		if(flags & O_CREAT){
			time_t t= time(NULL);
			int start = findOpenEntrySlot(parsed.parent);
			strcpy(parsed.parent[start].fileName,parsed.childName);
			parsed.parent[start].createDate=t;
			parsed.parent[start].lastAccessDate=t;
			parsed.parent[start].lastModifyDate=t;
			parsed.parent[start].fileSize = 0;
			parsed.parent[start].isFile = FILEMACRO;
		}
	}else{
		if(flags & O_TRUNC){
			parsed.parent[parsed.index].fileSize = 0;
		}
		if(flags & O_APPEND){
			//move file position to end
			pos=(parsed.parent[parsed.index].fileSize-1);
		}
	}
	fcbArray[returnFd].flags = flags;
	fcbArray[returnFd].buf = malloc(vcb->blockSize);
	fcbArray[returnFd].buflen = vcb->blockSize;
	fcbArray[returnFd].index = 0;
	fcbArray[returnFd].currentBlk = parsed.parent[parsed.index].location;
	fcbArray[returnFd].fileSize = parsed.parent[parsed.index].fileSize;
	fcbArray[returnFd].numBlocks = (parsed.parent[parsed.index].fileSize + vcb->blockSize - 1) / vcb->blockSize;
	fcbArray[returnFd].fileOffset = pos;
	return (returnFd); // all set
}

// Interface to seek function
int b_seek(b_io_fd fd, off_t offset, int whence)
{
	if (startup == 0)
		b_init(); // Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); // invalid file descriptor
	}
	if (whence == SEEK_SET)
	{
		//repositions the file offset to the offset passed by the caller.
		fcbArray[fd].fileOffset = offset;
	}
	else if (whence == SEEK_CUR)
	{
		//repositions the file offset to the current location plus caller offset bytes
		fcbArray[fd].fileOffset += offset;
	}
	else if (whence == SEEK_END) 
	{
		// repositions the file offset to the size of the file plus caller offset bytes
		fcbArray[fd].fileOffset = (fcbArray[fd].numBlocks * B_CHUNK_SIZE) + offset;
	}

	return fcbArray[fd].fileOffset; // Change this
}

// Interface to write function
int b_write(b_io_fd fd, char *buffer, int count)
{
	if (startup == 0)
		b_init(); // Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); // invalid file descriptor
	}

	return (0); // Change this
}

// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read(b_io_fd fd, char *buffer, int count)
{
	int part1, part2, part3;
	int leftover, blocks;
	if (startup == 0)
		b_init(); // Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); // invalid file descriptor
	}
	if(fcbArray[fd].flags & O_WRONLY){
		printf("This file cannot read\n");
		return -1;
	}
	if(count+fcbArray[fd].fileOffset>fcbArray[fd].fileSize){
		count = fcbArray[fd].fileSize - fcbArray[fd].fileOffset;
	}
	int remain = fcbArray[fd].buflen - fcbArray[fd].index;
	if(count>remain){
		part1 = remain;
		leftover = count - part1;
		blocks = leftover/vcb->blockSize;
		part2 = blocks*vcb->blockSize;
		part3 = leftover - part2;
	}else{
		part1 = count;
		fcbArray[fd].index+=part1;
		part2 = 0;
		part3 = 0;
	}
	LBAread(fcbArray[fd].buf,1,fcbArray[fd].currentBlk);
	
	if(part1>0){
		memcpy(buffer,fcbArray[fd].buf+fcbArray[fd].index,part1);
	}
	// if part2>0 it's larger than a blocksize
	if(part2>0){
		
		LBAread(buffer+part1,blocks,fcbArray[fd].currentBlk+1);
		
	}
	if(part3>0){
		LBAread(fcbArray[fd].buf,1,fcbArray[fd].currentBlk+blocks);
		fcbArray[fd].index=0;
		memcpy(buffer+part1+part2,fcbArray[fd].buf+fcbArray[fd].index,part3);
	}
	int bytesRead = part1+part2+part3;
	fcbArray[fd].fileOffset+=bytesRead;

	return count; // Change this
}

// Interface to Close the file
int b_close(b_io_fd fd)
{
}
