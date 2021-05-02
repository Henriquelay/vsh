#ifndef _LIST_H_
#define _LIST_H_

#include <sys/types.h>

typedef struct linked_node_t {
    pid_t sid;
    pid_t supervisor;
    struct linked_node_t* next;
    struct linked_node_t* previous;
} linked_node_t;

typedef struct list_t {
    linked_node_t* head;
    linked_node_t* tail;
    int mutex;
} list_t;

list_t* list_init();

int list_isEmpty(list_t* list);

pid_t list_pop(list_t* list);

void list_push(list_t* list, pid_t sid, pid_t supervisor);

void list_print(list_t* stack);

pid_t list_remove(list_t* list, pid_t sid);

pid_t list_removeNode(list_t *list, linked_node_t *node);

void list_destroy(list_t* list);

void list_runOnAll(list_t* list, void (*visit)(linked_node_t*));

#endif
