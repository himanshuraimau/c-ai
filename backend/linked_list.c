#include "linked_list.h"

struct LinkedList* create_list() {
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    list->head = list->tail = NULL;
    return list;
}

void add_message(struct LinkedList *list, const char *message) {
    struct ListNode *new_node = (struct ListNode *)malloc(sizeof(struct ListNode));
    new_node->message = strdup(message);
    new_node->next = NULL;
    if (list->tail == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

void free_list(struct LinkedList *list) {
    struct ListNode *current = list->head;
    struct ListNode *next;
    while (current != NULL) {
        next = current->next;
        free(current->message);
        free(current);
        current = next;
    }
    free(list);
}
