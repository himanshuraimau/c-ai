#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "ai.h"

#define CHAT_INPUT_MAX 1024
#define MAX_HISTORY 10000

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct ResponseData *resp = (struct ResponseData *)userp;
    
    char *ptr = realloc(resp->data, resp->size + realsize + 1);
    if(!ptr) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = 0;
    
    return realsize;
}

// Function to get user input
void get_user_input(char *buffer, int max_size) {
    printf("You: ");
    fgets(buffer, max_size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// Function to create request payload
char* create_json_payload(const char *input) {
    char *json = malloc(CHAT_INPUT_MAX * 2);
    snprintf(json, CHAT_INPUT_MAX * 2,
        "{"
        "\"contents\": [{"
        "    \"parts\": [{"
        "        \"text\": \"%s\""
        "    }]"
        "}]"
        "}", input);
    return json;
}

void init_ai() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void cleanup_ai() {
    curl_global_cleanup();
}

char* get_ai_response(const char* input) {
    CURL *curl;
    CURLcode res;
    struct ResponseData resp;
    const char *api_key = "";
    char url[256];
    
    resp.data = malloc(1);
    resp.size = 0;

    snprintf(url, sizeof(url), 
        "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=%s",
        api_key);
    
    curl = curl_easy_init();
    if(!curl) {
        free(resp.data);
        return strdup("Error initializing CURL");
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    char *json_data = create_json_payload(input);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
    
    res = curl_easy_perform(curl);
    
    free(json_data);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) {
        char *error = strdup(curl_easy_strerror(res));
        free(resp.data);
        return error;
    }

    return resp.data;
}