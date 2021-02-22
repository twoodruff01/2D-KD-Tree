/*
Contains all functions and structures for lists
List is a singly linked list with a pointer to a (struct entry.)
TODO: replace entry pointers with void pointers
*/
#ifndef LIST_H
#define LIST_H

struct listNode {
    struct entry *record;
    struct listNode *next;
};

struct listNode *createList();

void insertList(struct listNode **head, struct entry *data);

void freeList(struct listNode *head);

void writeList(struct listNode *list, FILE *outputData);
#endif
