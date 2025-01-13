#include <microhttpd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
#include "ai.h"

#define PORT 8080

struct PostContext {
    char *buffer;
    size_t size;
};

static enum MHD_Result handle_post_data(void *coninfo_cls, 
                                      enum MHD_ValueKind kind,
                                      const char *key,
                                      const char *filename,
                                      const char *content_type,
                                      const char *transfer_encoding,
                                      const char *data,
                                      uint64_t off,
                                      size_t size) {
    struct PostContext *context = (struct PostContext *)coninfo_cls;
    
    char *new_buffer = realloc(context->buffer, context->size + size + 1);
    if (!new_buffer)
        return MHD_NO;

    context->buffer = new_buffer;
    memcpy(context->buffer + context->size, data, size);
    context->size += size;
    context->buffer[context->size] = '\0';
    
    return MHD_YES;
}

static void request_completed(void *cls,
                            struct MHD_Connection *connection,
                            void **con_cls,
                            enum MHD_RequestTerminationCode toe) {
    struct PostContext *context = *con_cls;
    if (context) {
        if (context->buffer)
            free(context->buffer);
        free(context);
        *con_cls = NULL;
    }
}

static enum MHD_Result handle_request(void *cls,
                          struct MHD_Connection *connection,
                          const char *url,
                          const char *method,
                          const char *version,
                          const char *upload_data,
                          size_t *upload_data_size,
                          void **con_cls) {
    
    if (*con_cls == NULL) {
        struct PostContext *context = calloc(1, sizeof(struct PostContext));
        if (!context) return MHD_NO;
        *con_cls = context;
        return MHD_YES;
    }

    if (strcmp(method, "POST") == 0 && strcmp(url, "/chat") == 0) {
        struct PostContext *context = *con_cls;
        
        if (*upload_data_size != 0) {
            handle_post_data(*con_cls, 0, NULL, NULL, NULL, NULL,
                           upload_data, 0, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }

        // Parse JSON request
        struct json_object *parsed_json = json_tokener_parse(context->buffer);
        struct json_object *message_obj;
        json_object_object_get_ex(parsed_json, "message", &message_obj);
        const char *message = json_object_get_string(message_obj);

        // Get AI response
        char *ai_response = get_ai_response(message);
        
        // Create JSON response
        char *response_json;
        asprintf(&response_json, "{\"response\": %s}", ai_response);
        free(ai_response);

        struct MHD_Response *response = MHD_create_response_from_buffer(
            strlen(response_json),
            (void*)response_json,
            MHD_RESPMEM_MUST_FREE);

        MHD_add_response_header(response, "Content-Type", "application/json");
        MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
        
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        json_object_put(parsed_json);
        
        return ret;
    }

    struct MHD_Response *response;
    enum MHD_Result ret;
    
    // Handle CORS preflight
    if (strcmp(method, "OPTIONS") == 0) {
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
        MHD_add_response_header(response, "Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Serve static files for GET requests
    if (strcmp(method, "GET") == 0) {
        const char *filename = url;
        if (strcmp(url, "/") == 0) {
            filename = "/index.html";
        }
        
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "../frontend%s", filename);
        
        FILE *file = fopen(filepath, "rb");
        if (!file) {
            const char *not_found = "404 Not Found";
            response = MHD_create_response_from_buffer(strlen(not_found),
                                                     (void*)not_found,
                                                     MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *buffer = malloc(size);
        fread(buffer, 1, size, file);
        fclose(file);
        
        response = MHD_create_response_from_buffer(size, buffer, MHD_RESPMEM_MUST_FREE);
        
        // Set content type based on file extension
        if (strstr(filename, ".html")) {
            MHD_add_response_header(response, "Content-Type", "text/html");
        } else if (strstr(filename, ".css")) {
            MHD_add_response_header(response, "Content-Type", "text/css");
        } else if (strstr(filename, ".js")) {
            MHD_add_response_header(response, "Content-Type", "application/javascript");
        }
    } else {
        // Handle POST request for chat
        const char *json_response = "{\"response\": \"Hello from C server!\"}";
        response = MHD_create_response_from_buffer(strlen(json_response),
                                                 (void*)json_response,
                                                 MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "application/json");
    }
    
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    
    return ret;
}

int main() {
    init_ai();
    
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                            &handle_request, NULL,
                            MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
                            MHD_OPTION_END);
    
    if (daemon == NULL) {
        return 1;
    }
    
    printf("Server running on port %d\n", PORT);
    getchar();
    
    MHD_stop_daemon(daemon);
    cleanup_ai();
    return 0;
}