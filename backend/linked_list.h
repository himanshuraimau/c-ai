#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>

struct ListNode {
    char *message;
    struct ListNode *next;
};

struct LinkedList {
    struct ListNode *head;
    struct ListNode *tail;
};

struct LinkedList* create_list();
void add_message(struct LinkedList *list, const char *message);
void free_list(struct LinkedList *list);

#endif
