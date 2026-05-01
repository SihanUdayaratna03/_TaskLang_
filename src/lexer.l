%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int line_number = 1;
void yyerror(const char *s);
%}

%option noyywrap

%%

"TASK"      { return TASK; }
"RUN"       { return RUN; }
"EVERY"     { return EVERY; }
"DAY"       { return DAY; }
"WEEK"      { return WEEK; }
"ON"        { return ON; }
"AT"        { return AT; }
"AFTER"     { return AFTER; }
"IF"        { return IF; }
"success"   { return SUCCESS; }

"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return SEMICOLON; }

%%
