#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

bool is_number(char *val);

struct string
{
    char *value;
    size_t size;
    bool *expand;
};
void print_string(struct string *str);
/***
 * Frees a string struct
 * @param str : the string to free
 ***/
void string_destroy(struct string *str);
/***
 *  allocated a struct string copying the data from a static str
 *  it then frees the vaule and expand
 *  See finder(), pending has a non allaocted str
 * @param str : static string
 ***/
struct string *dup_str(struct string *str);

#endif /* !STRING_H */
