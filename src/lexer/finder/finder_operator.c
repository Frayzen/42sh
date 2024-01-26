#include "finder.h"
#include "io_backend/backend_saver.h"
#include "lexer/finder/finder_tools.h"

#include <assert.h>

void consume_redir_op(struct pending *p)
{
    char c = io_peek();
    // Append the first < or >
    append_io(p);
    char next = io_peek();
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
        append_io(p);
        if (io_peek() == c)
            append_io(p);
        break;
    case ';':
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
