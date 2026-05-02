#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

TaskList* create_task_list() {
    TaskList *list = (TaskList*)malloc(sizeof(TaskList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

Task* create_task(char *name) {
    Task *task = (Task*)malloc(sizeof(Task));
    task->name = name ? strdup(name) : NULL;
    task->script = NULL;
    task->has_condition = 0;
    task->schedule.time = NULL;
    task->schedule.weekday = NULL;
    task->schedule.depends_on = NULL;
    task->next = NULL;
    return task;
}

void append_task(TaskList *list, Task *task) {
    if (list->head == NULL) {
        list->head = task;
        list->tail = task;
    } else {
        list->tail->next = task;
        list->tail = task;
    }
}

void print_task_list(TaskList *list) {
    if (!list) return;
    printf("\n--- ABSTRACT SYNTAX TREE ---\n");
    Task *curr = list->head;
    while (curr) {
        printf("TASK: %s\n", curr->name ? curr->name : "NULL");
        printf("  RUN: %s\n", curr->script ? curr->script : "NULL");
        
        switch (curr->schedule.type) {
            case SCHED_DAILY:
                printf("  EVERY DAY AT %s\n", curr->schedule.time);
                break;
            case SCHED_WEEKLY:
                printf("  EVERY WEEK ON %s AT %s\n", curr->schedule.weekday, curr->schedule.time);
                break;
            case SCHED_TIMED:
                printf("  AT %s\n", curr->schedule.time);
                break;
            case SCHED_AFTER:
                printf("  AFTER %s\n", curr->schedule.depends_on);
                break;
        }
        
        if (curr->has_condition) {
            printf("  CONDITION: IF success\n");
        }
        
        printf("----------------------------\n");
        curr = curr->next;
    }
}
