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

struct lex_str
{
    char *value;
    size_t size;
    enum expand_type *expand;
};
void print_lex_str(struct lex_str *str);
/***
 * Frees a string struct
 * @param str : the string to free
 ***/
void destroy_lex_str(struct lex_str *str);
/***
 *  allocated a struct lex_str copying the data from a static str
 *  it then frees the vaule and expand
 *  See finder(), pending has a non allaocted str
 * @param str : static string
 ***/
struct lex_str *dup_lex_str(struct lex_str *str);

#endif /* !STRING_H */
