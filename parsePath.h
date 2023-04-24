#include "directoryEntry.h"
typedef struct
{
    int index;
    directoryEntry *parent;
} parsedPath;
void loadDirectory(directoryEntry *loadDir, directoryEntry *dir);
int locateEntry(directoryEntry *dir, char *entry);
parsedPath parsePath(char *path, directoryEntry *startPath);

int findOpenEntrySlot(directoryEntry *parent);
int fs_mkdir2(const char *pathname, mode_t mode);