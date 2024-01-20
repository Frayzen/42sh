#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

bool is_number(char *val);

struct string
{
    char *value;
    size_t size;
    int *expand;
};
void print_string(struct string *str);
void string_destory(struct string *str);
struct string *dup_str(struct string *str);

#endif /* !STRING_H */
