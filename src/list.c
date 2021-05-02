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

void list_push(list_t* list, pid_t sid, pid_t supervisor) {
    linked_node_t *newNode = (linked_node_t *)malloc(sizeof(linked_node_t));
    // printf("push %d\n", sid);
    if (newNode == NULL) {
        perror("Allocation error: Node couldn't be created. Exiting.");
        exit(1);
    }

    newNode->sid = sid;
    newNode->supervisor = supervisor;
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

pid_t list_remove(list_t *list, pid_t sid) {
    if (list == NULL) {
        return (pid_t)0;
    }
    if (list_isEmpty(list)) {
        return (pid_t)0;
    }

    linked_node_t *actual = list->head;
    while (actual != NULL) {
        if (actual->sid == sid) {
            if (!(actual == list->head)) { // not first
                actual->next->previous = actual->previous;
            }
            if (!(actual == list->tail)) { // not last
                actual->previous->next = actual->next;
            }
            free(actual);
            printf("removed %d\n", sid);
            return sid;
        }
    }
    return (pid_t)0;
}

pid_t list_removeNode(list_t *list, linked_node_t *node) {
    if (list == NULL) {
        return (pid_t)0;
    }
    if (list_isEmpty(list)) {
        return (pid_t)0;
    }

    if (!(node == list->head)) { // not first
        node->next->previous = node->previous;
    }
    if (!(node == list->tail)) { // not last
        node->previous->next = node->next;
    }
    pid_t sid = node->sid;
    free(node);
    return sid;
}

pid_t list_pop(list_t *list) {
    if (list->head == NULL) {
        return (pid_t)0;
    }

    pid_t holder = list->head->sid;
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
        printf("pid: %d\n", current->sid);
    }
    puts("");
}

void list_destroy(list_t *list) {
    if (list != NULL) {
        while (list->head != NULL) {
            linked_node_t *freeMe = list->head;
            printf("Freezand %d\n", freeMe->sid);
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
            printf("Visitando nó %d\n", useMe->sid);
            visit(useMe);
            useMe = useMeNext;
        }
    }
}
