#include "string.h"
#include <stdio.h>

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

void print_string(const struct string *str)
{
    if (!str || str->size == 0)
        return;
    for(size_t i = 0; i < str->size; i++)
        printf("|%c|", str->value[i]);
    for(size_t i = 0; i < str->size; i++)
        printf("|%d|", str->expand[i]);
    printf("\n\n");
}
