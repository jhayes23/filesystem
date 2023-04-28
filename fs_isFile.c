#include "mfs.h"
#include "directoryEntry.h"
#include "parsePath.h"

int fs_isFile(char *fileName)
{

    parsedPath path = parsePath(fileName);
    directoryEntry *dir = path.parent;

    if (dir->isFile == FILEMACRO)
    {
        return 1;
    }
    return 0;
}