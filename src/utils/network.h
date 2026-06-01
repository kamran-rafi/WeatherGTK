#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

typedef struct{
    char* data;
    size_t size;
} NetworkBuffer;

NetworkBuffer* fetch_api_data(const char* url);
void clear_network_buffer(NetworkBuffer* buffer);

#endif