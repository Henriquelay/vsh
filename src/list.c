#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list_t *list_init() {
    list_t *newList = (list_t *)malloc(sizeof(list_t));
    if (newList == NULL) {
        perror("Error creating new list. Exiting");
        exit(1);
    }
    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

int list_isEmpty(list_t *list) {
    return list->head == NULL;
}

void list_push(list_t *list, pid_t item) {
    linked_node_t *newNode = (linked_node_t *)malloc(sizeof(linked_node_t));
    // printf("push %d\n", item);
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

pid_t list_remove(list_t *list, pid_t item) {
    if (list == NULL) {
        return (pid_t)0;
    }
    if (list_isEmpty(list)) {
        return (pid_t)0;
    }

    linked_node_t *actual = list->head;
    while (actual != NULL) {
        if (actual->value == item) {
            if (!(actual == list->head)) { // not first
                actual->next->previous = actual->previous;
            }
            if (!(actual == list->tail)) { // not last
                actual->previous->next = actual->next;
            }
            free(actual);
            // printf("removed %d\n", item);
            return item;
        }
    }
    return (pid_t)0;
}

pid_t list_pop(list_t *list) {
    if (list->head == NULL) {
        return (pid_t)0;
    }

    pid_t holder = list->head->value;
    linked_node_t *destroyMe = list->head;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    } else {
        list->head->previous = NULL;
    }
    free(destroyMe);
    return holder;
}

void list_print(list_t *list) {
    for (linked_node_t *current = list->head; current != NULL; current = current->next) {
        printf("pid: %d\n", current->value);
    }
    puts("");
}

void list_destroy(list_t *list) {
    if (list != NULL) {
        while (list->head != NULL) {
            linked_node_t *freeMe = list->head;
            // printf("Freezand %d\n", freeMe->value);
            list->head = list->head->next;
            free(freeMe);
        }
        free(list);
    }
}

void list_runOnAll(list_t *list, void (*visit)(linked_node_t *)) {
    if (list != NULL) {
        linked_node_t *useMe = list->head;
        while (useMe != NULL) {
            linked_node_t *useMeNext = useMe->next;
            visit(useMe);
            useMe = useMeNext;
        }
    }
}
