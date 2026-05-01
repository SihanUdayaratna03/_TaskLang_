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

%type <task> task_definition
%type <task_list> program

%%
program: /* empty for now */ ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
