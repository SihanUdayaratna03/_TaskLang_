%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);
%}

%union {
    char *string;
    int number;
}

%token TASK RUN EVERY DAY WEEK ON AT AFTER IF SUCCESS
%token LBRACE RBRACE SEMICOLON
%token <string> IDENTIFIER STRING_LITERAL TIME WEEKDAY
%token <number> NUMBER

%%
program: /* empty for now */ ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
