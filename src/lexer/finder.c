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

// return -1 if not found, the id otherwise
ssize_t match_reserved(struct pending *p)
{
    if (!p || !p->value)
        return -1;
    size_t index = p->size - 1;
    for (size_t i = 0; i < TOK_TYPES_SIZE; i++)
    {
        if (!p->could_match[i])
            continue;
        const char *tok_str = TOK_TYPES_LT[i];
        if (tok_str[index] == p->value[index])
        {
            if (!tok_str[index + 1])
                return i;
        }
        else
            p->could_match[i] = false;
    }
    return -1;
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

void consumer(struct pending *p)
{
    while (true)
    {
        ssize_t match = match_reserved(p);
        if (match != -1)
            return;
        char c = io_peek();
        if (p->backslashed)
            goto append;
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
            continue;
        case ' ':
            if (!IS_BLANK(p))
                return;
            break;
        case '#':
            io_pop();
            append_until(p, '\n');
            continue;
        case '\n':
        case '\0':
        case ';':
            if (!IS_BLANK(p))
                return;
            /* FALLTHROUGH */
        default:
        append:
            append_char(p, c);
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
