#include "finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <threads.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer/finder/finder_tools.h"

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

// return true if pending is finished
void consume_quote(struct pending *p)
{
    p->blank = false;
    p->force_str = true;
    char c = io_peek();
    if (c == '\\')
    {
        p->backslashed = true;
        io_pop();
        return;
    }
    p->in_quote = true;
    io_pop();
    skip_until(p, c == '"' ? SKIP_DOUBLE_QUOTE : SKIP_SINGLE_QUOTE);
    if (!io_peek())
        exit_gracefully(UNEXPECTED_EOF);
    io_pop();
    p->in_quote = false;
}

bool check_next(void)
{
    switch (io_peek())
    {
    case '\'':
    case '\0':
    case '$':
    SPACE_CASES:
        return false;
    default:
        return true;
    }
}

void consume_variable(struct pending *p)
{
    io_pop();
    char c = io_peek();
    bool special_char = strchr(SPECIAL_PARAMETERS, c) != NULL;
    // If the dollar is not followed by any var
    if (c != '{' && !is_name_char(c) && !special_char)
    {
        append_char(p, '$');
        return;
    }
    // Enable expanding and begin the consumption of the name
    p->expanding = true;
    append_char(p, '$');
    if (special_char)
        append_io(p);
    else if (c == '{')
    {
        io_pop();
        skip_until(p, SKIP_VARIABLE_BRACKETS);
        if (io_peek() != '}')
            exit_gracefully(UNEXPECTED_EOF);
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
    p->expanding = false;
}

// return true if finished
bool consume(struct pending *p, char c)
{
    // Any external function in the switch should handle every pop involved
    switch (c)
    {
    case '\0':
        if (IS_BLANK(p))
            append_io(p);
        return true;
    OPERATORS_CASES:
        consume_operators(p);
        return true;
    QUOTE_CASES:
        consume_quote(p);
        return false;
    case '#':
        consume_comment(p);
        return false;
    case '$':
        consume_variable(p);
        return false;
    SPACE_CASES:
        if (!IS_BLANK(p))
            return true;
        io_pop();
        break;
    default:
        append_io(p);
        break;
    }
    return false;
}

void consumer(struct pending *p)
{
    while (true)
    {
        char c = io_peek();
        if (p->backslashed)
        {
            p->backslashed = false;
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
    p.blank = true;
    consumer(&p);
    // Append the null terminating char
    struct lex_str *str = &p.str;
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = '\0';
    return &p;
}
