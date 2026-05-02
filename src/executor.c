#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "executor.h"

#define MAX_TASKS 64

static Task *sorted[MAX_TASKS];
static int sorted_count = 0;
static char visited_names[MAX_TASKS][64];
static int visited_count = 0;

static int is_visited(const char *name) {
    for (int i = 0; i < visited_count; i++) {
        if (strcmp(visited_names[i], name) == 0) return 1;
    }
    return 0;
}

static Task *find_task(TaskList *list, const char *name) {
    Task *curr = list->head;
    while (curr) {
        if (curr->name && strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static void topo_visit(TaskList *list, Task *task) {
    if (!task || !task->name) return;
    if (is_visited(task->name)) return;

    if (task->schedule.type == SCHED_AFTER && task->schedule.depends_on) {
        Task *dep = find_task(list, task->schedule.depends_on);
        if (dep) topo_visit(list, dep);
    }

    strncpy(visited_names[visited_count++], task->name, 63);
    sorted[sorted_count++] = task;
}
