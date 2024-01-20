#include "string.h"

#include <stdio.h>
#include <stdlib.h>

bool is_number(char *val)
{
    int i = 0;
    while (val[i])
    {
        if (val[i] > '9' || val[i] < '0')
            return false;
        i++;
    }
    return true;
}

void print_string(struct string *str)
{
    if (!str || str->size == 0)
        return;
    for (size_t i = 0; i < str->size; i++)
        printf("|%c|", str->value[i]);
    printf("\n");
}

void string_destory(struct string *str)
{
    if (!str)
        return;
    free(str->value);
    free(str->expand);
    free((void *)str);
}

struct string *dup_str(struct string *str)
{
    struct string *res = calloc(1, sizeof(struct string));
    res->value = malloc(sizeof(char) *str->size + 1);
    res->size = str->size;
    for(size_t i = 0 ; i < str->size + 1; i++)
        res->value[i] = str->value[i];
    return res;
}
