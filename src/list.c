#include <stdlib.h>
#include <stdio.h>
#include "list.h"

list_t* list_init() {
    list_t* newList = (list_t*)malloc(sizeof(list_t));
    if (newList == NULL) {
        perror("Error creating new list. Exiting");
        exit(1);
    }
    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

int list_isEmpty(list_t* list) {
    return list->head == NULL;
}

void list_push(list_t* list, pid_t item) {
    linked_node_t* newNode = (linked_node_t*)malloc(sizeof(linked_node_t));
    if (newNode == NULL) {
        perror("Allocation error: Node couldn't be created. Exiting.");
        exit(1);
    }

    newNode->value = item;
    newNode->previous = NULL;
    newNode->next = list->head;
    if (newNode->next != NULL) {
        newNode->next->previous = newNode;
    }
    list->head = newNode;
    if (list->tail == NULL) {
        list->tail = newNode;
    }
}

pid_t list_remove(list_t* list, pid_t item) {
    if (list->head == NULL) {
        return NULL;
    }

    linked_node_t* actual = list->head;
    if(list_isEmpty(list)){
        return (pid_t)0;
    }
    while(actual != NULL) {
        if(actual->value == item){
            if(actual == list->head){
                list->head = NULL;
                if(actual == list->tail){
                    list->tail = NULL;
                }else{
                    list->head = actual->next;
                    list->head->previous = NULL;                    
                }
            }else if(actual == list->tail){
                list->tail = actual->previous;
                list->tail->next = NULL;
            }else{
                actual->previous->next = actual->next;
                actual->next->previous = actual->previous;
            }
            free(actual);
            return item;    
        }
    }
    return (pid_t)0;
}

void list_print(list_t* list) {
    for (linked_node_t* current = list->head; current != NULL; current = current->next) {
        printf("pid: %d\n", current->value);
    }
    puts("");
}

void list_destroy(list_t* list) {
    if (list != NULL) {
        while (list->head != NULL) {
            linked_node_t* freeMe = list->head;
            list->head = list->head->next;
            free(freeMe);
        }
        free(list);
    }
}
