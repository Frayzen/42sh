#include <assert.h>
#include <stdio.h>

#include "finder.h"
#include "io_backend/backend_saver.h"
#include "lexer/finder/finder_tools.h"

static char get_next_char(void)
{
    while (io_peek() == '\\')
    {
        io_pop();
        if (io_peek() != '\n')
            break;
        io_pop();
    }
    return io_peek();
}

void consume_redir_op(struct pending *p)
{
    char c = io_peek();
    // Append the first < or >
    append_io(p);
    char next = get_next_char();
    switch (c)
    {
    case '>':
        if (next == '>' || next == '&' || next == '|')
            append_io(p);
        break;
    case '<':
        if (next == '&' || next == '>')
            append_io(p);
        break;
    default:
        assert(false);
        break;
    }
}

void consume_control_op(struct pending *p)
{
    char c = io_peek();
    switch (c)
    {
    AND_OR_CASES:
    case ';':
        append_io(p);
        if (get_next_char() == c)
            append_io(p);
        break;
    case '(':
    case ')':
    case '\n':
        append_io(p);
        break;
    default:
        assert(false);
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
    default:
        assert(false);
        break;
    }
}
