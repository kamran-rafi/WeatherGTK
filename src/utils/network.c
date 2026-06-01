/*
 * Functionality for Making HTTP Requests is in this file.
*/

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "network.h"

// This callback function is passed to cURL lib to write data to NetworkBuffer object.
static size_t buffer_write_callback(void* contents, size_t size, size_t nmemb, void* userp){
    size_t total_size = size * nmemb;
    NetworkBuffer* mem = (NetworkBuffer*)userp;

    // Allocate/reallocate memory for the incoming data chunk plus null terminator
    char* ptr = realloc(mem->data, mem->size + total_size + 1);
    if(ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->data = ptr;
    // Copy the new data chunk into our memory array
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0'; // Null-terminate

    return total_size;
}

/*
 * fetches data from API endpoint
*/
NetworkBuffer* fetch_api_data(const char* url){
    log_debug("Fetching API data from endpoint: %s", url);

    // Create an initial empty buffer.
    NetworkBuffer* buffer = malloc(sizeof(NetworkBuffer));
    buffer->data = malloc(1);
    buffer->size = 0;

    // Initial CURL setup.
    curl_global_init(CURL_GLOBAL_ALL);
    CURLcode res; // Store response code
    CURL* curl = curl_easy_init();

    if(curl){
        // Set the API URL from where data is fetched. 
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Follow HTTP redirects if any occur
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the callback function to handle data writing
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buffer_write_callback);
        // Pass our memory struct to the callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
        // Perform the network transfer synchronously
        res = curl_easy_perform(curl);

        // Check for transfer errors
        if(res != CURLE_OK) {
            log_debug("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            clear_network_buffer(buffer);
            curl_easy_cleanup(curl);
            curl_global_cleanup(); // Clear the memory.
            return NULL;
        }

        // Clean up the curl session.
        curl_easy_cleanup(curl);

    }
    else{
        log_error("cURL failed to initialize.");
        clear_network_buffer(buffer);
        curl_global_cleanup(); // Clear the memory.
        return NULL;
    }

    curl_global_cleanup(); // Clear the memory.

    return buffer;
}

// Cleans up the NetworkBuffer from heap memory after usage.
void clear_network_buffer(NetworkBuffer* buffer){
    if(buffer){
        // Clear the memory of data inside the buffer.
        if(buffer->data) free(buffer->data);

        // Clear the buffer.
        free(buffer);
        buffer = NULL; // To prevent dangling pointer.
    }
}