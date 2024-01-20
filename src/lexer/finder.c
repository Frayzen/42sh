#include "finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <threads.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"

#define APPEND_CHARS true
#define SPACE_CASES                                                            \
    case ' ':                                                                  \
    case '\f':                                                                 \
    case '\r':                                                                 \
    case '\t':                                                                 \
    case '\v'

// return the p->value
char *append_char(struct pending *p, char c, bool expand)
{
    // if (p-> == NULL)
        // printf("fuck\n");
    struct string *str = &(p->str);
    str->value = realloc(str->value, ++str->size);
    str->value[str->size - 1] = c;
    str->expand = realloc(str->expand, sizeof(int) *str->size);

    str->expand[str->size - 1] = expand;
    p->blank = false;
    return str->value;
}

// Append every char until limit is found (limit excluded)
void skip_until(struct pending *p, char limit, bool append, bool expand)
{
    char c = io_peek();
    while (c && c != limit)
    {
        if (append)
            append_char(p, c, expand);
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
    case '=':
        if (IS_BLANK(p))
        {
            append_char(p, c, false);
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

// return true if the chevron is accepted
bool chevron(struct pending *p, char c)
{
    append_char(p, c, false);
    io_pop();
    char next = io_peek();
    switch (c)
    {
    case '>':
        if (next == '>' || next == '&' || next == '|')
        {
            append_char(p, next, false);
            io_pop();
        }
        break;
    case '<':
        if (next == '&' || next == '>')
        {
            append_char(p, next, false);
            io_pop();
        }
        break;
    default:
        return false;
    }
    return true;
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

bool handle_dollar(struct pending *p)
{

    io_pop();
    char next = io_peek();
    switch (next)
    {
        case '\'': 
        case '\0':
        SPACE_CASES:
            append_char(p, '$', false);
            return false;

        case ('{'):
            io_pop();
            char ch = io_peek();
            while(ch != '\0' && ch != '}')
            {
                append_char(p, io_peek(), true);
                io_pop();
            }
            return false; 

        default:
            append_char(p, '$', true);
            if (io_peek() == '$')
            {    
                append_char(p, '$', true);
                return false;
            }
            while (check_next())
            {
                append_char(p, io_peek(), true);
                io_pop();
            }
            return false;
    }
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
        skip_until(p, c, APPEND_CHARS,);
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
    case '>':
    case '<':
        if (IS_BLANK(p) && !chevron(p, c))
            goto append;
        return true;
    case '$':
        return handle_dollar(p);
    SPACE_CASES:
    case '\n':
    case '\0':
    case ';':
    case '=':
        return special_char(p);
    default:
    append:
        append_char(p, c, false);
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
            append_char(p, c, false);
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
    p.in_quote = false;
    consumer(&p);
    append_char(&p, '\0', false);
    p.str.size--;
    return &p;
}
