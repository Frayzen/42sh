#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_exp_str(struct exp_str *str)
{
    if (!str || str->size == 0)
        return;
    for (size_t i = 0; i < str->size; i++)
        printf("|%c|", str->value[i]);
    printf("\n");
}

void destroy_exp_str(struct exp_str *str)
{
    if (!str)
        return;
    if (str->value)
        free(str->value);
    if (str->expand)
        free(str->expand);
    free(str);
}

struct exp_str *dup_exp_str(struct exp_str *str)
{
    // str is provided from the pending struct so we dont need to free it
    // there is probably a better way to do it ;)
    struct exp_str *res = calloc(1, sizeof(struct exp_str));
    res->size = str->size;
    res->value = malloc(sizeof(char) * (str->size + 1));
    memcpy(res->value, str->value, sizeof(char) * (str->size + 1));
    res->expand = malloc(sizeof(enum expand_type) * str->size);
    memcpy(res->expand, str->expand, sizeof(enum expand_type) * str->size);
    free(str->expand);
    free(str->value);
    return res;
}
