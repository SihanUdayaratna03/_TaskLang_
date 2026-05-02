#include <stdio.h>
#include "y.tab.h"
#include "ast.h"

extern FILE* yyin;
extern int yyparse();
extern TaskList *root_list;

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    printf("Starting parser...\n");
    if (yyparse() == 0) {
        printf("Parsing successful!\n");
        print_task_list(root_list);
    } else {
        printf("Parsing failed.\n");
    }

    if (argc > 1) {
        fclose(yyin);
    }

    return 0;
}
