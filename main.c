#include "lexer.h"

// Example usage:
void parse_functions(Vector functions) {
    VECTOR_FOR_EACH(Function, func, &functions) {
        switch (func->instruction) {
            case I_PUSH:
                {
                    printf("push %d\n", atoi(vector_get_str(&func->args, 0)));
                    break;
                }
            case I_ADD:
                {
                    printf("add\n");
                    break;
                }
            case I_SUB:
                {
                    printf("sub\n");
                    break;
                }
            case I_MUL:
                {
                    printf("mul\n");
                    break;
                }
            case I_DIV:
                {
                    printf("div\n");
                    break;
                }

            default:
                {
                    printf("Not handled instruction: %d\n", func->instruction);
                    break;
                }
        }

    }
}


int main() {
    Vector result = lexer("test.ztest");
    if (result.size == 0) {
        return 1;
    }
    parse_functions(result);


    return 0;

}