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

static void print_schedule(Task *task) {
    switch (task->schedule.type) {
        case SCHED_DAILY:
            printf("  Schedule : EVERY DAY AT %s\n", task->schedule.time);
            break;
        case SCHED_WEEKLY:
            printf("  Schedule : EVERY WEEK ON %s AT %s\n",
                   task->schedule.weekday, task->schedule.time);
            break;
        case SCHED_TIMED:
            printf("  Schedule : AT %s\n", task->schedule.time);
            break;
        case SCHED_AFTER:
            printf("  Depends  : AFTER %s\n", task->schedule.depends_on);
            if (task->has_condition) {
                printf("  Condition: IF success\n");
            }
            break;
    }
}

void execute_plan(TaskList *list) {
    sorted_count = 0;
    visited_count = 0;

    Task *curr = list->head;
    while (curr) {
        topo_visit(list, curr);
        curr = curr->next;
    }

    printf("\n--- EXECUTION START ---\n");
    for (int i = 0; i < sorted_count; i++) {
        Task *t = sorted[i];
        printf("\nExecuting Task : %s\n", t->name);
        printf("  Script       : \"%s\"\n", t->script ? t->script : "");
        print_schedule(t);
        printf("  Status       : [QUEUED]\n");
    }
    printf("\n--- EXECUTION COMPLETE ---\n");
}
