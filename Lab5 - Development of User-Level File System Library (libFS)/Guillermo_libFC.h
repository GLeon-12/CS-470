#ifndef LIBS_H
#define LIBS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_FILE_SIZE 1024

// File system structures
typedef struct {
    char filename[MAX_FILENAME];
    int size;
    int is_open;
} FileEntry;

// Function prototypes
int fileCreate(const char *filename);
int fileOpen(const char *filename);
int fileWrite(int file_index, const char *data);
int fileRead(int file_index, void *buffer, int buffer_size);
int fileClose(int file_index); 
int fileDelete(const char *filename);

#endif