#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

bool is_number(char *val);

enum expand_type
{
    STR_LITTERAL = 0,
    QUOTED_VAR,
    UNQUOTED_VAR,
};

struct exp_str
{
    char *value;
    size_t size;
    enum expand_type *expand;
};
void print_exp_str(struct exp_str *str);
/***
 * Frees a string struct
 * @param str : the string to free
 ***/
void destroy_exp_str(struct exp_str *str);
/***
 *  allocated a struct exp_str copying the data from a static str
 *  it then frees the vaule and expand
 *  See finder(), pending has a non allaocted str
 * @param str : static string
 ***/
struct exp_str *dup_exp_str(struct exp_str *str);

#endif /* !STRING_H */
