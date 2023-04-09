typedef struct directoryEntry
{
    // size of struct is 136 bytes
    int id; //unique ID that is associated with the directoryEntry
    int isFile; //boolean value that's assocaited whether the entry is a file or directory. 
    int location; //stores block location of file
    int fileSize; //the file size in bytes
    time_t createDate; //timestamp associated with when the file was created by FS
    time_t lastAccessDate; //timestamp associated when the file was last accessed by FS
    time_t lastModifyDate; //timestamp associated when the last write of a file occurs
    char fileName[64]; //string with max 64 characters that contains the file name
    char author[32]; //string with max 32 characters that contains the author name of the file.
} directoryEntry;