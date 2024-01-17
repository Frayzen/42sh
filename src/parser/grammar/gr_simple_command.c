#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_simple_command(struct ast **ast)
{
    struct ast *ast_cmd = init_ast(AST_COMMAND, NULL);
    if (gr_prefix(&ast_cmd) == ERROR)
    {
        struct token *token = tok_peek();
        CHECK_GOTO(!IS_BUILTIN(token) && token->type != WORD, error);
        tok_pop();
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
    else
    {
        enum status state = OK;
        while (state == OK)
        {
            if (tok_peek()->terminal)
                break;
            state = gr_prefix(&ast_cmd);

        }
        struct token *token = tok_peek();
        if (!IS_BUILTIN(token) && token->type != WORD)
        {
            *ast = add_child(*ast, ast_cmd);
            return OK;
        }
        tok_pop();
        struct ast *new_ast = init_ast(AST_TOKEN, token);
        ast_cmd = add_child(ast_cmd, new_ast);
        state = OK;
        while (state == OK)
        {
            if (tok_peek()->terminal)
                break;
            state = gr_element(&ast_cmd);
        }
        *ast = add_child(*ast, ast_cmd);
        return OK;
    }
error:
    destroy_ast(ast_cmd);
    return ERROR;
}
