#include "finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "io_backend/backend_saver.h"
#include "tools/token/token.h"

// return the p->value
char *append_char(struct pending *p, char c)
{
    p->value = realloc(p->value, ++p->size);
    p->value[p->size - 1] = c;
    return p->value;
}

// Append every char until limit is found (limit excluded)
void append_until(struct pending *p, char limit)
{
    char c = io_peek();
    while (c && c != limit)
    {
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
    case ' ':
        if (IS_BLANK(p))
            io_pop();
        else
            return true;
        break;
    default:
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
            goto append;
        }
        /*
         * Any external function in the switch should handle every pop involved
         */
        switch (c)
        {
        case '\\':
            p->backslashed = true;
            break;
        case '\'':
            io_pop();
            append_until(p, c);
            io_pop();
            continue;
        case '#':
            io_pop();
            append_until(p, '\n');
            continue;
        case ' ':
        case '\n':
        case '\0':
        case ';':
            if (special_char(p))
                return;
            continue;
        default:
        append:
            append_char(p, c);
            break;
        }
        io_pop();
    }
}

char *finder(void)
{
    static struct pending p;
    // reset the pending structure
    memset(&p, 0, sizeof(struct pending));
    memset(p.could_match, true, TOK_TYPES_SIZE);
    consumer(&p);
    append_char(&p, '\0');
    return p.value;
}
