#include "finder_tools.h"

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

bool is_portable_char(char c)
{
    if (c == '\0')
        return true;
    if (c >= '\a' && c <= '\r')
        return true;
    if (c >= ' ' && c <= '~')
        return true;
    return false;
}

bool is_name(char *str, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        char c = str[i];
        // Check if c is in the portable character set
        if (is_portable_char(c))
            continue;
        return false;
    }
    return true;
}

bool assignment_word(const struct exp_str *str)
{
    char *begin = str->value;
    char *first_eq = strchr(begin, '=');
    if (!first_eq)
        return false;
    size_t size = first_eq - begin + 1;
    return is_name(begin, size);
}

// Also pop
void append_char(struct pending *p)
{
    struct exp_str *str = &p->str;
    str->value = realloc(str->value, ++str->size);
    str->value[str->size - 1] = io_peek();
    str->expand = realloc(str->expand, str->size);
    str->expand[str->size - 1] = p->expanding;
    io_pop();
    p->blank = false;
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
            append_char(p);
        }
        else
            io_pop();
        c = io_peek();
    }
}
