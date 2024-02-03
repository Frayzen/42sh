#include "finder.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer/finder/finder_tools.h"
#include "tools/definitions.h"

void consume_comment(struct pending *p)
{
    if (IS_BLANK(p))
    {
        io_pop();
        skip_until(p, SKIP_HASHTAG);
    }
    else
        append_io(p);
}

// return TRUE_B if pending is finished
void consume_quote(struct pending *p)
{
    p->blank = FALSE_B;
    p->force_str = TRUE_B;
    char c = io_peek();
    if (c == '\\')
    {
        p->backslashed = TRUE_B;
        io_pop();
        return;
    }
    p->in_quote = TRUE_B;
    io_pop();
    skip_until(p, c == '"' ? SKIP_DOUBLE_QUOTE : SKIP_SINGLE_QUOTE);
    if (!io_peek())
        exit_gracefully(UNEXPECTED_EOF);
    io_pop();
    p->in_quote = FALSE_B;
}

int check_next(void)
{
    switch (io_peek())
    {
    case '\'':
    case '\0':
    case '$':
    SPACE_CASES:
        return FALSE_B;
    default:
        return TRUE_B;
    }
}

void consume_variable(struct pending *p)
{
    io_pop();
    char c = io_peek();
    int special_char = strchr(SPECIAL_PARAMETERS, c) != NULL;
    // If the dollar is not followed by any var
    if (c != '{' && !is_name_char(c) && !special_char)
    {
        append_char(p, '$');
        return;
    }
    // Enable expanding and begin the consumption of the name
    p->expanding = TRUE_B;
    append_char(p, '$');
    if (special_char)
        append_io(p);
    else if (c == '{')
    {
        io_pop();
        skip_until(p, SKIP_VARIABLE_BRACKETS);
        if (io_peek() != '}')
        {
            if (!io_peek())
                exit_gracefully(UNEXPECTED_EOF);
        }
        io_pop();
    }
    else
    {
        while (is_name_char(c))
        {
            append_io(p);
            c = io_peek();
        }
    }
    p->expanding = FALSE_B;
}

// return TRUE_B if finished
int consume(struct pending *p, char c)
{
    // Any external function in the switch should handle every pop involved
    switch (c)
    {
    case '\0':
        if (IS_BLANK(p))
            append_io(p);
        return TRUE_B;
    OPERATORS_CASES:
        consume_operators(p);
        return TRUE_B;
    QUOTE_CASES:
        consume_quote(p);
        return FALSE_B;
    case '#':
        consume_comment(p);
        return FALSE_B;
    case '$':
        consume_variable(p);
        return FALSE_B;
    SPACE_CASES:
        if (!IS_BLANK(p))
            return TRUE_B;
        io_pop();
        break;
    default:
        append_io(p);
        break;
    }
    return FALSE_B;
}

void consumer(struct pending *p)
{
    while (TRUE_B)
    {
        char c = io_peek();
        if (p->backslashed)
        {
            p->backslashed = FALSE_B;
            if (c != '\n')
                append_io(p);
            else
                io_pop();
        }
        else if (consume(p, c))
            return;
    }
}

struct pending *finder(void)
{
    static struct pending p;
    // reset the pending structure
    memset(&p, 0, sizeof(struct pending));
    p.blank = TRUE_B;
    consumer(&p);
    // Append the null terminating char
    struct lex_str *str = &p.str;
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = '\0';
    return &p;
}
