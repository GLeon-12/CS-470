#include "Guillermo_libFC.h"

// Global file system
FileEntry file_table[MAX_FILES];
int file_count = 0;

// Create a new file
int fileCreate(const char *filename) {

    // Check if file already exists
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            printf("Error: File '%s' already exists.\n", filename);
            return -1;
        }
    }

    // Create the file on the local disk
    FILE *fp = fopen(filename, "w");
    if (!fp){
        printf("Error: Unable to create file '%s'.\n", filename);
        return -1;
    }
    fclose(fp);

    // Add file to the file table
    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].size = 0;
    file_table[file_count].is_open = 0;
    file_count++;

    printf("File '%s' created successfully.\n", filename);
    return 0;
}

// Open a file
int fileOpen(const char *filename) {
    // Find the file in the file table
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if (file_table[i].is_open) {
                printf("Error: File '%s' is already open.\n", filename);
                return -1;
            }
            // Mark the file as open
            file_table[i].is_open = 1;
            printf("File '%s' opened successfully.\n", filename);
            return i; // Return file index
        }
    }
    // File not found
    printf("Error: File '%s' not found.\n", filename);
    return -1;
}

// Write data to a file
int fileWrite(int file_index, const char *data) {
    // Check if file index is valid and file is open
    if (!file_table[file_index].is_open){ 
        printf("Error: File is not open for writing.\n");
        return -1;
    }

    int data_size = strlen(data); 

    // Open the file in write mode
    FILE *file = fopen(file_table[file_index].filename, "w");
    if (!file) {
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }

    // Write data and close
    fwrite(data, 1, data_size, file);
    fclose(file);

    // Update file size
    file_table[file_index].size = data_size;
    printf("Data written to file '%s' successfully.\n", file_table[file_index].filename);
    return 0;
}


// Read data from a file
int fileRead(int file_index, void *buffer, int buffer_size) {
    // Check if file index is valid
    if (!file_table[file_index].is_open) {
        printf("Error: File '%s' is not open for reading.\n", file_table[file_index].filename);
        return -1;
    }
    
    // Read data from the file on the local disk
    FILE *file = fopen(file_table[file_index].filename, "r");
    if (!file) {
        printf("Error: Unable to open file '%s' for reading.\n", file_table[file_index].filename);
        return -1;
    }
    // Read data into the buffer and close the file
    int bytes_read = fread(buffer, 1, buffer_size, file);
    fclose(file);

    // Print success message
    printf("Data read from file '%s' successfully.\n", file_table[file_index].filename);
    return bytes_read;
}

// Close a file
int fileClose(int file_index) {
    // Check if file index is valid and file is open
    if (!file_table[file_index].is_open) {
        printf("Error: File '%s' is not open.\n", file_table[file_index].filename);
        return -1;
    }
    // Mark the file as closed and print success message
    file_table[file_index].is_open = 0;
    printf("File '%s' closed successfully.\n", file_table[file_index].filename);
    return 0;
}

// Delete a file
int fileDelete(const char *filename) {
    // Find the file in the file table and check if it's open 
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if (file_table[i].is_open) {
                // Print error message if file is open and cannot be deleted
                printf("Error: File '%s' is currently open and cannot be deleted.\n", filename);
                return -1;
            }
            // Remove the file from the local disk 
            if (remove(filename) != 0) {
                printf("Error: Unable to delete file '%s'.\n", filename);
                return -1;
            }
            // Shift remaining files in the table
            for (int j = i; j < file_count - 1; j++) {
                file_table[j] = file_table[j + 1];
            }
            // Decrement file count and print success message
            file_count--;
            printf("File '%s' deleted successfully.\n", filename);
            return 0;
        }
    }
    // Print error message if file is not found
    printf("Error: File '%s' not found.\n", filename);
    return -1;
}