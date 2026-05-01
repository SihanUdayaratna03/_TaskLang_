%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
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

%type <task> task_definition task_body
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
    RUN STRING_LITERAL SEMICOLON { $$ = create_task(NULL); $$->script = $2; }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
