#ifndef AST_H
#define AST_H

typedef enum {
    SCHED_DAILY,
    SCHED_WEEKLY,
    SCHED_TIMED,
    SCHED_AFTER
} ScheduleType;

typedef struct {
    ScheduleType type;
    char *time;
    char *weekday;
    char *depends_on;
} Schedule;

typedef struct Task {
    char *name;
    char *script;
    Schedule schedule;
    int has_condition;
    struct Task *next;
} Task;

typedef struct {
    Task *head;
    Task *tail;
} TaskList;

TaskList* create_task_list();
void append_task(TaskList *list, Task *task);
Task* create_task(char *name);
void print_task_list(TaskList *list);

#endif
