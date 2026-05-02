#include <stdio.h>
#include <string.h>
#include "semantic.h"

int check_duplicate_tasks(TaskList *list) {
    Task *outer = list->head;
    while (outer) {
        Task *inner = outer->next;
        while (inner) {
            if (outer->name && inner->name && strcmp(outer->name, inner->name) == 0) {
                fprintf(stderr, "Semantic Error: Duplicate task name '%s' defined.\n", outer->name);
                return 1;
            }
            inner = inner->next;
        }
        outer = outer->next;
    }
    return 0;
}

int check_undefined_tasks(TaskList *list) {
    Task *curr = list->head;
    while (curr) {
        if (curr->schedule.type == SCHED_AFTER && curr->schedule.depends_on) {
            int found = 0;
            Task *search = list->head;
            while (search) {
                if (search->name && strcmp(search->name, curr->schedule.depends_on) == 0) {
                    found = 1;
                    break;
                }
                search = search->next;
            }
            if (!found) {
                fprintf(stderr, "Semantic Error: Task '%s' references undefined task '%s'.\n",
                        curr->name, curr->schedule.depends_on);
                return 1;
            }
        }
        curr = curr->next;
    }
    return 0;
}

static Task* find_task_by_name(TaskList *list, const char *name) {
    Task *curr = list->head;
    while (curr) {
        if (curr->name && strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static int dfs(TaskList *list, Task *curr, char visited[][64], char in_stack[][64], int *count) {
    strncpy(visited[*count], curr->name, 63);
    strncpy(in_stack[*count], curr->name, 63);
    (*count)++;

    if (curr->schedule.type == SCHED_AFTER && curr->schedule.depends_on) {
        int on_stack = 0;
        for (int i = 0; i < *count; i++) {
            if (strcmp(in_stack[i], curr->schedule.depends_on) == 0) {
                on_stack = 1;
                break;
            }
        }
        if (on_stack) return 1;

        Task *next = find_task_by_name(list, curr->schedule.depends_on);
        if (next && dfs(list, next, visited, in_stack, count)) return 1;
    }

    strncpy(in_stack[*count - 1], "", 63);
    (*count)--;
    return 0;
}

int check_circular_dependencies(TaskList *list) {
    char visited[64][64] = {0};
    char in_stack[64][64] = {0};
    Task *curr = list->head;
    while (curr) {
        int count = 0;
        if (dfs(list, curr, visited, in_stack, &count)) {
            fprintf(stderr, "Semantic Error: Circular dependency detected involving task '%s'.\n", curr->name);
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

int run_semantic_analysis(TaskList *list) {
    if (!list) return 1;
    if (check_duplicate_tasks(list)) return 1;
    if (check_undefined_tasks(list)) return 1;
    if (check_circular_dependencies(list)) return 1;
    return 0;
}
