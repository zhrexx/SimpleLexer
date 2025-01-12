#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

// TODO: Change name to instruction.
typedef struct {
    char *name;
    Vector args;
} Function;

void free_function(Function *func) {
    free(func->name);
    vector_free(&func->args);
}

int lexer(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", file_path);
        return 1;
    }

    char buffer[8192];
    size_t bytes_read = fread(buffer, sizeof(char), 8191, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);

    Vector functions;
    vector_init(&functions, 10, sizeof(Function));

    Vector lines = split_to_vector(buffer, "\n");
    VECTOR_FOR_EACH(char *, line, &lines) {
        if (strlen(*line) == 0) continue;

        Vector tokens = split_to_vector(*line, " ");
        if (tokens.size < 1) {
            vector_free(&tokens);
            continue;
        }

        char *function_name = strdup(vector_get_str(&tokens, 0));
        vector_remove(&tokens, 0);

        Function function__ = {
            .name = function_name,
            .args = tokens 
        };

        if (strcmp("push", function_name) == 0 && tokens.size == 1) {
            vector_push(&functions, &function__);
        } else {
            fprintf(stderr, "Error: Unknown function or too few arguments: %s\n", function_name);
            free_function(&function__); 
            exit(1);
        }
    }

    // Todo implement parsing
    VECTOR_FOR_EACH(Function, func, &functions) {
        printf("Function: %s\n", func->name);
        VECTOR_FOR_EACH(char *, arg, &func->args) {
            printf("  Arg: %s\n", *arg);
        }
    }

    VECTOR_FOR_EACH(Function, func, &functions) {
        free_function(func);
    }
    vector_free(&functions);
    vector_free(&lines);

    return 0;
}

int main() {

    int result = lexer("test.ztest");
    if (result != 0) {
        return result;
    }

    return 0;

}

