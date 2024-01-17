#include "finder.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "exit/exit.h"
#include "io_backend/backend_saver.h"

#define APPEND_CHARS true
#define SPACE_CASES                                                            \
    case ' ':                                                                  \
    case '\f':                                                                 \
    case '\r':                                                                 \
    case '\t':                                                                 \
    case '\v'

// return the p->value
char *append_char(struct pending *p, char c)
{
    struct string *str = &p->str;
    str->value = realloc(str->value, ++str->size);
    str->value[str->size - 1] = c;
    p->blank = false;
    return str->value;
}

// Append every char until limit is found (limit excluded)
void skip_until(struct pending *p, char limit, bool append)
{
    char c = io_peek();
    while (c && c != limit)
    {
        if (append)
            append_char(p, c);
        io_pop();
        c = io_peek();
    }
}

// Special are \n \0 space and ;
// return true if pending is over
bool special_char(struct pending *p)
{
    char c = io_peek();
    switch (c)
    {
    case '\n':
    case '\0':
    case ';':
        if (IS_BLANK(p))
        {
            append_char(p, c);
            io_pop();
        }
        return true;
    SPACE_CASES:
        if (!IS_BLANK(p))
            return true;
        io_pop();
        break;
    default:
        break;
    }
    return false;
}

// return true if finished
bool consume(struct pending *p, char c)
{
    // Any external function in the switch should handle every pop involved
    switch (c)
    {
    case '\\':
        p->backslashed = true;
        break;
    case '\'':
    case '"':
        p->blank = false;
        p->force_word = true;
        io_pop();
        skip_until(p, c, APPEND_CHARS);
        if (!io_peek())
            exit_gracefully(UNEXPECTED_EOF);
        io_pop();
        return false;
    case '#':
        if (IS_BLANK(p))
        {
            io_pop();
            skip_until(p, '\n', !APPEND_CHARS);
            io_pop();
        }
        else
            goto append;
        return false;
    SPACE_CASES:
    case '\n':
    case '\0':
    case ';':
        return special_char(p);
    default:
    append:
        append_char(p, c);
        break;
    }
    io_pop();
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
            append_char(p, c);
            io_pop();
        }
        else if (consume(p, c))
            return;
    }
}

const struct pending *finder(void)
{
    static struct pending p;
    // reset the pending structure
    memset(&p, 0, sizeof(struct pending));
    p.blank = true;
    consumer(&p);
    append_char(&p, '\0');
    p.str.size--;
    return &p;
}
