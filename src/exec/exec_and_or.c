#include <assert.h>

#include "exec/commands/execs_cmd.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

// return the ret value or -1 on ERROR
int exec_and_or_child(struct ast *child)
{
    switch (child->type)
    {
    case AST_PIPE:
        return exec_pipe(AST_PIPE(child));
    case AST_AND_OR:
        return exec_and_or(AST_AND_OR(child));
    default:
        exit_gracefully(LIST_NOT_FOUND);
        return -1;
    }
}

#define SUCCESS 0
int exec_and_or(struct ast_and_or *ast)
{
    assert(AST(ast)->type == AST_AND_OR);
    struct ast_list *list = AST_LIST(ast);
    assert(list && list->nb_children > 0);
    int prev = exec_and_or_child(list->children[0]);
    for (int i = 1; i < list->nb_children; i++)
    {
        struct ast *child = list->children[i];
        // PREVIOUS ONE
        enum token_type type = ast->types[i - 1];
        if (type == AND && prev != SUCCESS)
            continue;
        if (type == OR && prev == SUCCESS)
            continue;
        prev = exec_and_or_child(child);
        if (prev == -1)
            return 1;
    }
    return prev;
}
