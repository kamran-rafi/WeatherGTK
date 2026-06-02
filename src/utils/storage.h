#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

char* get_content(char* file);
void write_content(const char* file, const char* data);
void clear_contents(char* buffer);

#endif