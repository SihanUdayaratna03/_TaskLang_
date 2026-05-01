%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int line_number;
void yyerror(const char *s);

TaskList *root_list = NULL;
%}

%union {
    char *string;
    int number;
    Task *task;
    TaskList *task_list;
}

%token TASK RUN EVERY DAY WEEK ON AT AFTER IF SUCCESS
%token LBRACE RBRACE SEMICOLON
%token <string> IDENTIFIER STRING_LITERAL TIME WEEKDAY
%token <number> NUMBER

%type <task> task_definition task_body schedule_spec condition_spec
%type <task_list> program task_list

%%
program:
    task_list { root_list = $1; }
;

task_list:
    task_definition { $$ = create_task_list(); append_task($$, $1); }
  | task_list task_definition { append_task($1, $2); $$ = $1; }
;

task_definition:
    TASK IDENTIFIER LBRACE task_body RBRACE { $$ = $4; $$->name = $2; }
;

task_body:
    RUN STRING_LITERAL SEMICOLON schedule_spec { $$ = $4; $$->script = $2; }
  | RUN STRING_LITERAL SEMICOLON schedule_spec condition_spec { $$ = $4; $$->script = $2; $$->has_condition = 1; }
;

schedule_spec:
    EVERY DAY AT TIME SEMICOLON { $$ = create_task(NULL); $$->schedule.type = SCHED_DAILY; $$->schedule.time = $4; }
  | EVERY WEEK ON WEEKDAY AT TIME SEMICOLON { $$ = create_task(NULL); $$->schedule.type = SCHED_WEEKLY; $$->schedule.weekday = $4; $$->schedule.time = $6; }
  | AT TIME SEMICOLON { $$ = create_task(NULL); $$->schedule.type = SCHED_TIMED; $$->schedule.time = $2; }
  | AFTER IDENTIFIER SEMICOLON { $$ = create_task(NULL); $$->schedule.type = SCHED_AFTER; $$->schedule.depends_on = $2; }
;

condition_spec:
    IF SUCCESS SEMICOLON { $$ = NULL; }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_number, s);
}
