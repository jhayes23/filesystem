#include "mfs.h"
#include "directoryEntry.h"
#include "parsePath.h"

int fs_isFile(char *filename)
{

    parsedPath path = parsePath(filename);
    directoryEntry *dir = path.parent;

    if (dir->isFile == FILEMACRO)
    {
        return 1;
    }
    return 0;
}