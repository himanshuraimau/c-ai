#include "linked_list.h"

struct LinkedList* create_list() {
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    list->head = list->tail = NULL; // Initialize head and tail to NULL
    return list;
}

void add_message(struct LinkedList *list, const char *message) {
    struct ListNode *new_node = (struct ListNode *)malloc(sizeof(struct ListNode));
    new_node->message = strdup(message); // Duplicate the message
    new_node->next = NULL;
    if (list->tail == NULL) {
        list->head = list->tail = new_node; // If list is empty, set head and tail to new node
    } else {
        list->tail->next = new_node; // Otherwise, append to the end
        list->tail = new_node;
    }
}

void free_list(struct LinkedList *list) {
    struct ListNode *current = list->head;
    struct ListNode *next;
    while (current != NULL) {
        next = current->next;
        free(current->message); // Free the message
        free(current); // Free the node
        current = next;
    }
    free(list); // Free the list structure
}

char* get_chat_history(struct LinkedList *list, int max_messages) {
    char *history = malloc(10000); // Allocate reasonable buffer
    history[0] = '\0';
    
    struct ListNode *current = list->head;
    int count = 0;
    
    while (current != NULL && count < max_messages) {
        if (count % 2 == 0) {
            strcat(history, "User: ");
        } else {
            strcat(history, "Assistant: ");
        }
        strcat(history, current->message); // Append message to history
        strcat(history, "\n");
        current = current->next;
        count++;
    }
    
    return history;
}
