#include "lexer/token_saver.h"
#include <stddef.h>
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_simple_command(struct ast** ast)
{
    struct ast *ast_cmd = init_ast(AST_COMMAND, NULL);
    enum status state = gr_element(&ast_cmd);
    if (state == ERROR)
    {
        destroy_ast(ast_cmd);
        return ERROR;
    }
    while (state == OK)
    {
        state = gr_element(&ast_cmd);
    }
    *ast = add_child(*ast, ast_cmd);
    return OK;

}
