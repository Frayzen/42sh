#include "args.h"
#include <stdlib.h>
#include "tools/ast/ast.h"

struct arg* expand()
{

}

void parse_arg(struct ast_cmd *cmd, struct exp_str *arg)
{
    struct args *cmd;
    for (int i = 0; i < arg->size; i++)
    {
        if (arg->expand[i])
        {
            parse_arg(expand(arg));
        }
    }

}

