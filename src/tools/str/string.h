#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

bool is_number(char *val);

struct string
{
    char *value;
    size_t size;
};

#endif /* !STRING_H */
