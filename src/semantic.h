#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

int check_duplicate_tasks(TaskList *list);
int check_undefined_tasks(TaskList *list);
int check_circular_dependencies(TaskList *list);
int run_semantic_analysis(TaskList *list);

#endif
