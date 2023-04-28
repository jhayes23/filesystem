#include "mfs.h"
#include "directoryEntry.h"
#include "parsePath.h"

int fs_isDir(char *fileName)
{

    parsedPath path = parsePath(fileName);
    directoryEntry *dir = path.parent;

    if (dir->isFile == DIRECTORY)
    {
        return 1;
    }
    return 0;
}