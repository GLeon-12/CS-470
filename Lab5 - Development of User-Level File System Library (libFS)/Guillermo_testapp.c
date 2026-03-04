#include "Guillermo_libFC.h"
#include <ctype.h>

// Function to display the menu options
void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Create File\n");
    printf("2. Open File\n");
    printf("3. Write to File\n");
    printf("4. Read from File\n");
    printf("5. Close File\n");
    printf("6. Delete File\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Function to wait for user input before continuing
void waitForUser() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
}

int main() { 
    // Variables to hold user input and file information
    char filename[MAX_FILENAME];
    char introduction[] = "Welcome to my File System Test Application! This application will involve libFS, a user-level library that emulates key functionalities of a file system. "
                            "It allows applications to perform file and directory operations by linking with it. This library interacts with the terminal and simulates disk operations to manage data at the disk level.\n\n"
                            "In this application, you can create text files, write messages into them, read the contents, close files, and delete them safely. "
                            "The program ensures that files are handled correctly, preventing operations on files that are not open or already in use. "
                            "Through this project, I learned how file systems manage data and how applications interact with them.";
    // Variable to keep track of the currently open file index
    int file_index = -1;
    int choice;

    // Main loop to display the menu and process user choices
    while(1) {
        
        // Calling the function to display the menu and get user input
        displayMenu();
        scanf("%d", &choice);
        getchar(); 

        // Processing user choices using a switch statement
        switch (choice) {
            case 1: // Create File

                // Prompting user for filename and creating the file
                printf("Enter filename to create: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0'; 
               
                if (fileCreate(filename) != 0) {
                    printf("Error creating file.\n");
                }   
                waitForUser();
                break;


            case 2: // Open File
                
                // Prompting user for filename and opening the file
                printf("Enter filename to open: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0'; 
                file_index = fileOpen(filename);

                if (file_index == -1) {
                    printf("Error opening file.\n");
                }   
                waitForUser();
                break;

            case 3: // Write to File
                
                // Checking if a file is currently open before writing else show an error message
                if (file_index == -1) {
                    printf("Error: No file is currently open.\n");
                    
                } else {
                    printf("Writing introduction message to file '%s'...\n", filename);
                    if (fileWrite(file_index, introduction) != 0) {
                        printf("Error writing to file.\n");
                    }
                }

                waitForUser();
                break;

            case 4: // Read from File

                // Checking if a file is currently open before reading else show an error message
                if (file_index == -1) {
                    printf("Error: No file is currently open.\n");
                    break;
                }

                // Reading data from the file and displaying it to the user 
                char buffer[MAX_FILE_SIZE];
                int bytes_read = fileRead(file_index, buffer, sizeof(buffer));
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; 
                    printf("Data read from file:\n%s\n", buffer);
                }
                waitForUser();
                break;

            case 5: // Close File

                // Checking if a file is currently open before closing else show an error message 
                if (file_index == -1) {
                    printf("Error: No file is currently open.\n");
                    break;
                }

                // Closing the currently open file and resetting the file index
                fileClose(file_index);
                file_index = -1; 
                waitForUser();
                break;

            case 6: // Delete File
                
                // Prompting user for filename and deleting the file
                printf("Enter filename to delete: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0; 
                fileDelete(filename);
                waitForUser();
                break;

            case 7: // Exit
                printf("Exiting the program. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                waitForUser();
                break;
        }
    }
}