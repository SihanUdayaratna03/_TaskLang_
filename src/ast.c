#include <stdlib.h>
#include <string.h>
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
