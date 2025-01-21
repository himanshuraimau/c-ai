#ifndef AI_H
#define AI_H

#include <curl/curl.h>

// Define the struct completely in the header
struct ResponseData {
    char *data; // Response data
    size_t size; // Size of the response data
};

// Update function declaration to include history parameter
char* get_ai_response(const char* input, const char* history); // Function to get AI response
void cleanup_ai(); // Function to cleanup AI resources
void init_ai(); // Function to initialize AI resources

#endif
