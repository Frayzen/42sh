#include "finder_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "finder.h"
#include "io_backend/backend_saver.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

bool is_terminating(enum token_type type)
{
    switch (type)
    {
    case NEWLINE:
    case SEMI_COLON:
    case BSZERO:
        return true;
    default:
        return false;
    }
}

bool chevron_type(const struct exp_str *str)
{
    if (!strcmp(">", str->value))
        return 1;
    if (!strcmp("<", str->value))
        return 1;
    if (!strcmp(">>", str->value))
        return 1;
    if (!strcmp(">&", str->value))
        return 1;
    if (!strcmp("<&", str->value))
        return 1;
    if (!strcmp(">|", str->value))
        return 1;
    if (!strcmp("<>", str->value))
        return 1;
    return 0;
}

bool is_name_char(char c)
{
    if (c == '_')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

bool is_name(char *str, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        char c = str[i];
        // Check if c is in the portable character set
        if (is_name_char(c))
            continue;
        return false;
    }
    return true;
}

bool assignment_word(const struct exp_str *str)
{
    char *begin = str->value;
    char *first_eq = strchr(begin, '=');
    // If no equal has been found or equal is the first char
    if (!first_eq && first_eq != str->value)
        return false;
    size_t size = first_eq - begin;
    return is_name(begin, size);
}

void append_char(struct pending *p, char c)
{
    struct exp_str *str = &p->str;
    size_t id = str->size++;
    str->value = realloc(str->value, str->size * sizeof(char));
    str->value[id] = c;
    str->expand = realloc(str->expand, str->size * sizeof(enum expand_type));
    if (!p->expanding)
        str->expand[id] = STR_LITTERAL;
    else if (p->in_quote)
        str->expand[id] = QUOTED_VAR;
    else
        str->expand[id] = UNQUOTED_VAR;
    p->blank = false;
}

// Also pop
void append_io(struct pending *p)
{
    append_char(p, io_peek());
    io_pop();
}

// Append every char until limit is found (limit excluded)
void skip_until(struct pending *p, char limit, bool append)
{
    char c = io_peek();
    while (c && c != limit)
    {
        if (append)
        {
            if (limit == '"' && c == '$')
                consume_variable(p);
            else
                append_io(p);
        }
        else
            io_pop();
        c = io_peek();
    }
}
