#include "finder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer_tools.h"

#define APPEND_CHARS true

#define SPACE_CASES                                                            \
    case ' ':                                                                  \
    case '\f':                                                                 \
    case '\r':                                                                 \
    case '\t':                                                                 \
    case '\v'

#define QUOTE_CASES                                                            \
    case '\'':                                                                 \
    case '"':                                                                  \
    case '\\'

#define REDIR_OPS_CASES                                                        \
    case '<':                                                                  \
    case '>'

#define AND_OR_CASES                                                           \
    case '&':                                                                  \
    case '|'

#define CONTROL_OP_CASES                                                       \
    AND_OR_CASES:                                                              \
    case ';':                                                                  \
    case '\n'
/* TODO include ( and ) */

#define OPERATORS_CASES                                                        \
    REDIR_OPS_CASES:                                                           \
    CONTROL_OP_CASES

// TODO redo that to make it readable
void consume_and_or(struct pending *p, char c)
{
    append_char(p);
    if (IS_BLANK(p))
    {
        append_char(p);
        io_pop();
        if (io_peek() == c)
        {
            append_char(p);
            io_pop();
        }
    }
}

void consume_redir_op(struct pending *p)
{
    char c = io_peek();
    // Append the first < or >
    append_char(p);
    char next = io_peek();
    switch (c)
    {
    case '>':
        if (next == '>' || next == '&' || next == '|')
            append_char(p);
        break;
    case '<':
        if (next == '&' || next == '>')
            append_char(p);
        break;
    }
}

void consume_control_op(struct pending *p)
{
    char c = io_peek();
    switch (c)
    {
    AND_OR_CASES:
        consume_and_or(p, c);
        break;
    case ';':
    case '\n':
        append_char(p);
        break;
    }
}

void consume_operators(struct pending *p)
{
    if (!IS_BLANK(p))
        return;
    char c = io_peek();
    switch (c)
    {
    REDIR_OPS_CASES:
        consume_redir_op(p);
        break;
    CONTROL_OP_CASES:
        consume_control_op(p);
        break;
    }
}

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

const struct pending *finder(void)
{
    static struct pending p;
    // reset the pending structure
    memset(&p, 0, sizeof(struct pending));
    p.blank = true;
    consumer(&p);
    // Append the null terminating char
    struct string *str = &p.str;
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = '\0';
    return &p;
}
