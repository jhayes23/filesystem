typedef struct directoryEntry
{
    // size of struct is 136 bytes
    int id;
    int isFile;
    int location;
    int fileSize;
    time_t createDate;
    time_t lastAccessDate;
    time_t lastModifyDate;
    char fileName[64];
    char author[32];
} directoryEntry;