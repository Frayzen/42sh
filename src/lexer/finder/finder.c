#include "finder.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <threads.h>

#include "exit/error_handler.h"
#include "finder/finder_tools.h"
#include "io_backend/backend_saver.h"


void consume_comment(struct pending *p)
{
    if (IS_BLANK(p))
    {
        io_pop();
        skip_until(p, '\n', !APPEND_CHARS);
    }
    else
        append_char(p);
}

// return true if pending is finished
void consume_quote(struct pending *p)
{
    char c = io_peek();
    if (c == '\\')
    {
        p->backslashed = true;
        return;
    }
    append_char(p);
    skip_until(p, c, APPEND_CHARS);
    if (!io_peek())
        exit_gracefully(UNEXPECTED_EOF);
    append_char(p);
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
    append_char(p);
    char c = io_peek();
    p->expanding = true;
    if (c == '{')
        skip_until(p, '}', APPEND_CHARS);
    else
    {
        while (is_portable_char(c))
        {
            append_char(p);
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
            append_char(p);
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
    SPACE_CASES:
        if (!IS_BLANK(p))
            return true;
        io_pop();
        break;
    default:
        append_char(p);
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
            append_char(p);
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
    struct exp_str *str = &p.str;
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = '\0';
    return &p;
}
