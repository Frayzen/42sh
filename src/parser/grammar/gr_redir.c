
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_io_file(struct ast **ast)
{
    struct token *token = tok_peek();
    CHECK_GOTO(token->type != CHEVRON, error);
    tok_pop();
    struct ast *chevron = init_ast(AST_TOKEN, token);
    *ast = add_child(*ast, chevron);
    token = tok_peek();
    CHECK_GOTO(!IS_WORDABLE(token), error);
    struct ast *new_ast = init_ast(AST_TOKEN, token);
    tok_pop();
    *ast = add_child(*ast, new_ast);
    return OK;
error:
    return ERROR;
}

enum status gr_redir(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->type != IO_NUMBER)
        return ERROR;
    tok_pop();
    struct ast *redir_ast = init_ast(AST_REDIR, NULL);
    struct ast *optional = init_ast(AST_TOKEN, token);
    redir_ast = add_child(redir_ast, optional);
    if (gr_io_file(&redir_ast) == OK)
    {
        *ast = add_child(*ast, redir_ast);
        return OK;
    }
    optional->token = NULL;
    destroy_ast(redir_ast);
    optional = init_ast(AST_TOKEN, token);
    add_child(*ast, optional);
    return OK;
}
