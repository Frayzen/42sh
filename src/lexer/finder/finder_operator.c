#include "finder.h"
#include "finder/finder_tools.h"
#include "io_backend/backend_saver.h"

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
        append_char(p);
        if (io_peek() == c)
            append_char(p);
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
