/*
 * The code in this file is used for saving and loading data from files.
 */

#include "stdio.h"
#include "logger.h"
#include "stdlib.h"

/*
 * This function uses file pointer of opened file and 
 * returns the length of its content.
*/
long get_content_length(FILE* fptr) {
    if(!fptr){
        log_error("Failed to get content length. NULL file passed.");
        return -1;
    }
    fseek(fptr, 0, SEEK_END);   // Move to the end of file.
    long length = ftell(fptr);                // Get and save the position of file into length variable.
    rewind(fptr);                           // Reset pointer to the beginning for further use.
    return length;
}

/*
 * Open a file from file path string and returns the
 * dynamically allocated character array of characters.
*/
char* get_content(const char* file){ 
    FILE* fptr = fopen(file, "r"); 
    if(!fptr){
        log_error("Failed to load the file.");
        return NULL;
    }

    int fileLength = get_content_length(fptr);
    if(fileLength == -1){
        log_error("Failed to load the file.");
        return NULL;
    }

    // Add 1 for null terminator.
    char* buffer = malloc(fileLength + 1);
    buffer[fileLength] = '\0'; // Attach null terminator at end of buffer.

    // Load the file content to buffer.
    fread(buffer, 1, fileLength, fptr);

    fclose(fptr);

    return buffer;
}

/*
 * Used to write data fetched from API to a local file to used a cache later.
*/
void write_content(const char* file, const char* data){
    FILE* fptr = fopen(file, "w"); 
    if(!fptr){
        log_error("Failed to open the file.");
        return;
    }

    // Write to file.
    fputs(data, fptr);
    
    // Close the file to clear up memory.
    fclose(fptr);

    log_debug("Wrote file %s", file);
}

// This function is used to free up the content from heap memory.
void clear_contents(char* buffer){
    if(buffer){
        free(buffer);
        buffer = NULL;
    }
}