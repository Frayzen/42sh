#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

bool is_number(char *val);

struct exp_str
{
    char *value;
    size_t size;
    bool *expand;
};
void print_string(struct exp_str *str);
/***
 * Frees a string struct
 * @param str : the string to free
 ***/
void string_destroy(struct exp_str *str);
/***
 *  allocated a struct exp_str copying the data from a static str
 *  it then frees the vaule and expand
 *  See finder(), pending has a non allaocted str
 * @param str : static string
 ***/
struct exp_str *dup_str(struct exp_str *str);

#endif /* !STRING_H */
