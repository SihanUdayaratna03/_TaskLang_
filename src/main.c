#include <stdio.h>
#include "ast.h"
#include "y.tab.h"
#include "semantic.h"

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

    printf("Parsing TaskLang++ input...\n");
    if (yyparse() != 0) {
        printf("Parsing failed.\n");
        return 1;
    }

    if (run_semantic_analysis(root_list) != 0) {
        return 1;
    }

    printf("Semantic Analysis Passed.\n");
    print_task_list(root_list);

    if (argc > 1) {
        fclose(yyin);
    }

    return 0;
}
