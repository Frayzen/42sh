#include <stddef.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_simple_command(struct ast **ast)
{
    struct token *token = tok_peek();
    if (!IS_BUILTIN(token) && token->type != WORD)
        return ERROR;
    tok_pop();
    struct ast *ast_cmd = init_ast(AST_COMMAND, NULL);
    struct ast *new_ast = init_ast(AST_TOKEN, token);
    ast_cmd = add_child(ast_cmd, new_ast);
    enum status state = OK;
    while (state == OK)
    {
        if (tok_peek()->terminal)
            break;
        state = gr_element(&ast_cmd);
    }
    *ast = add_child(*ast, ast_cmd);
    return OK;
}
