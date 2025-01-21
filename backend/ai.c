#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "ai.h"
#include <json-c/json.h>

#define CHAT_INPUT_MAX 20480 // Max input size
#define MAX_HISTORY 40000 // Max history size
#define API_MAX_TOKENS 307200 // API max tokens


// Define the struct completely in the source file

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size *nmemb;
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

// Update create_json_payload to accept history
char* create_json_payload(const char *input, const char *history) {
    // Calculate approximate size needed - use 4x for safety
    size_t needed_size = strlen(history) + strlen(input) + 256;
    char *json = malloc(needed_size);
    
    // Ensure we're not exceeding reasonable limits
    if (strlen(history) > MAX_HISTORY) {
        // Truncate history from the end to keep most recent
        history = history + strlen(history) - MAX_HISTORY;
    }
    
    snprintf(json, needed_size,
        "{"
        "\"contents\": [{"
        "    \"parts\": [{"
        "        \"text\": \"Previous conversation:\\n%s\\nUser: %s\""
        "    }]"
        "}]"
        "}", history, input);
    return json;
}

void init_ai() {
    curl_global_init(CURL_GLOBAL_ALL); // Initialize CURL
}

void cleanup_ai() {
    curl_global_cleanup(); // Cleanup CURL
}

// Update get_ai_response to use history
char* get_ai_response(const char* input, const char* history) {
    CURL *curl;
    CURLcode res;
    struct ResponseData resp;
    const char *api_key = ""; // API key
    char url[256];
    
    resp.data = malloc(1);
    resp.size = 0;

    snprintf(url, sizeof(url), 
        "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-pro:generateContent?key=%s",
        api_key);
    
    curl = curl_easy_init();
    if(!curl) {
        free(resp.data);
        return strdup("Error initializing CURL");
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    char *json_data = create_json_payload(input, history);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
    
    res = curl_easy_perform(curl);
    
    
    printf("Raw API Response:\n%s\n", resp.data);
    
    free(json_data);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) {
        char *error = strdup(curl_easy_strerror(res));
        free(resp.data);
        return error;
    }

    // Parse the JSON response to extract the actual message
    struct json_object *parsed_json = json_tokener_parse(resp.data);
    if (!parsed_json) {
        free(resp.data);
        return strdup("Error parsing JSON response");
    }

    // Navigate through the JSON structure
    struct json_object *candidates, *first_candidate, *content, *parts, *first_part, *text;
    
    json_object_object_get_ex(parsed_json, "candidates", &candidates);
    first_candidate = json_object_array_get_idx(candidates, 0);
    json_object_object_get_ex(first_candidate, "content", &content);
    json_object_object_get_ex(content, "parts", &parts);
    first_part = json_object_array_get_idx(parts, 0);
    json_object_object_get_ex(first_part, "text", &text);

    // Get the actual response text
    const char *response_text = json_object_get_string(text);
    char *final_response = strdup(response_text);

    // Cleanup
    json_object_put(parsed_json);
    free(resp.data);

    return final_response;
}