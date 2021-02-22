/*
Contains all functions for lists/nodes in a singly linked list.
TODO: replace entry pointer with void pointer
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "entry.h"

/* 
Returns pointer to new list. 
*/
struct listNode *createList() {
    return NULL;
}

/*
Inserts new listNode into list, and pointer to struct entry.
We don't need to worry about coordinates; they're inside the struct.
*/
void insertList(struct listNode **head, struct entry *data) {

    struct listNode *temp = malloc(sizeof(struct listNode));
    assert(temp);
    temp->record = data;
    temp->next = *head;
    // point head to the new first listNode: which is temp
    *head = temp;
}

/*
Frees all listNodes in list, and frees their struct entries. 
*/
void freeList(struct listNode *head) {

    struct listNode *p = head;
    while (p) {  
        freeEntry(p->record);
        struct listNode *temp = p;
        p = p->next;
        free(temp);
    }
}

/*
Write all entries in list to the given file.
*/
void writeList(struct listNode *list, FILE *outputData) {

    struct listNode *temp = list;
    while (temp) {
        writeStructToFile(temp->record, outputData);
        temp = temp->next;
    }
}