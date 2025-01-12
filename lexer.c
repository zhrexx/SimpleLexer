#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "str.h"

// TODO: Change name to instruction.

typedef enum {
    I_PUSH,
    I_ADD,
    I_SUB,
    I_DIV,
    I_MUL
} Instructions;

Instructions parse_instruction(char *instruction) {
    if (strcmp("push", instruction) == 0) return I_PUSH;
    if (strcmp("add", instruction) == 0) return I_ADD;
    if (strcmp("sub", instruction) == 0) return I_SUB;
    if (strcmp("div", instruction) == 0) return I_DIV;
    if (strcmp("mul", instruction) == 0) return I_MUL;
    return -1;
}

char *instruction_to_string(Instructions instruction) {
    switch (instruction) {
        case I_PUSH: return "push";
        case I_ADD: return "add";
        case I_SUB: return "sub";
        case I_DIV: return "div";
        case I_MUL: return "mul";
        default: return "unknown";
    }
}

int instruction_expects_args(Instructions instruction) {
    switch (instruction) {
        case I_PUSH: return 1;
        case I_ADD: return 0;
        case I_SUB: return 0;
        case I_DIV: return 0;
        case I_MUL: return 0;
        default: return -1;
    }
}


typedef struct {
    Instructions instruction;
    Vector args;
} Function;

void free_function(Function *func) {
    // free(func->instruction);
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
        if (strlen(*line) == 0 || starts_with("//", *line)) continue;

        Vector tokens = split_to_vector(*line, " ");
        if (tokens.size < 1) {
            vector_free(&tokens);
            continue;
        }

        char *function_name = strdup(vector_get_str(&tokens, 0));
        vector_remove(&tokens, 0);
        Instructions parsed_instruction = parse_instruction(function_name);

        Function function__ = {
            .instruction = parsed_instruction,
            .args = tokens 
        };

        if (parsed_instruction == I_PUSH && tokens.size == 1) {
            vector_push(&functions, &function__);
        } else if (parsed_instruction == I_ADD && tokens.size == 0) {
            vector_push(&functions, &function__);
        } else if (parsed_instruction == I_SUB && tokens.size == 0) {
            vector_push(&functions, &function__);
        } else if (parsed_instruction == I_DIV && tokens.size == 0) {
            vector_push(&functions, &function__);
        } else if (parsed_instruction == I_MUL && tokens.size == 0) {
            vector_push(&functions, &function__);
        } else {
            fprintf(stderr, "Error: Unknown instruction or too few arguments(this expects %d): %s\n", instruction_expects_args(parsed_instruction), function_name);
            free_function(&function__); 
            exit(1);
        }
    }

    // Todo implement parsing
    VECTOR_FOR_EACH(Function, func, &functions) {
        printf("Function: %s\n", instruction_to_string(func->instruction));
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

