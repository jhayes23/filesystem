#include "mfs.h"
#include "directoryEntry.h"
#include "parsePath.h"

int fs_isDir(char *filename)
{

    parsedPath path = parsePath(filename);
    directoryEntry *dir = path.parent;

    if (dir->isFile == DIRECTORY)
    {
        return 1;
    }
    return 0;
}