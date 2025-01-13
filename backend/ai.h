#ifndef AI_H
#define AI_H

#include <curl/curl.h>

// Define the struct completely in the header
struct ResponseData {
    char *data;
    size_t size;
};

char* get_ai_response(const char* input);
void cleanup_ai();
void init_ai();

#endif
