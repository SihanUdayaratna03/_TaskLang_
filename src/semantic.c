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
    return 0;
}

int check_circular_dependencies(TaskList *list) {
    return 0;
}

int run_semantic_analysis(TaskList *list) {
    if (!list) return 1;
    if (check_duplicate_tasks(list)) return 1;
    if (check_undefined_tasks(list)) return 1;
    if (check_circular_dependencies(list)) return 1;
    return 0;
}
