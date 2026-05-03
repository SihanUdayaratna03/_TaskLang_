%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

int line_number = 1;
void yyerror(const char *s);
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT}|_)*
STRING      \"[^\"]*\"
TIME        {DIGIT}{DIGIT}:{DIGIT}{DIGIT}
WHITESPACE  [ \t\r]

%%

{WHITESPACE}+   { }
\n              { line_number++; }
"//".*          { }

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

"MONDAY"        { yylval.string = strdup(yytext); return WEEKDAY; }
"TUESDAY"       { yylval.string = strdup(yytext); return WEEKDAY; }
"WEDNESDAY"     { yylval.string = strdup(yytext); return WEEKDAY; }
"THURSDAY"      { yylval.string = strdup(yytext); return WEEKDAY; }
"FRIDAY"        { yylval.string = strdup(yytext); return WEEKDAY; }
"SATURDAY"      { yylval.string = strdup(yytext); return WEEKDAY; }
"SUNDAY"        { yylval.string = strdup(yytext); return WEEKDAY; }

{IDENTIFIER}    { yylval.string = strdup(yytext); return IDENTIFIER; }
{STRING}        { char *str = malloc(strlen(yytext)-1); strncpy(str, yytext+1, strlen(yytext)-2); str[strlen(yytext)-2]='\0'; yylval.string = str; return STRING_LITERAL; }
{TIME}          { yylval.string = strdup(yytext); return TIME; }

.               { fprintf(stderr, "Lexical Error at line %d: Unrecognized character '%c'\n", line_number, yytext[0]); exit(1); }

%%
