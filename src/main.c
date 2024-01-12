#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/token/token.h"

int main(void)
{
    char *str = "false";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    printf("%d %d\n", token->type, T_FALSE);
    printf("Hello World!\n");
}
