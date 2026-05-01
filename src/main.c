#include <stdio.h>
#include "y.tab.h"

extern int yylex();
extern char* yytext;
extern FILE* yyin;

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    int token;
    while ((token = yylex())) {
        printf("Token: %d, Lexeme: %s\n", token, yytext);
    }

    if (argc > 1) {
        fclose(yyin);
    }

    return 0;
}
