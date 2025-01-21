#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>

struct ListNode {
    char *message; // Message content
    struct ListNode *next; // Pointer to the next node
};

struct LinkedList {
    struct ListNode *head; // Head of the list
    struct ListNode *tail; // Tail of the list
};

struct LinkedList* create_list(); // Function to create a new list
void add_message(struct LinkedList *list, const char *message); // Function to add a message to the list
void free_list(struct LinkedList *list); // Function to free the list

// Add this new function declaration
char* get_chat_history(struct LinkedList *list, int max_messages); // Function to get chat history

#endif
