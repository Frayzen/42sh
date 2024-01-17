#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"
/*
int check_chevron(char *value)
{
    if (!strcmp(value, ">"))
        return 1;
    if (!strcmp(value, "<"))
        return 1;
    if (!strcmp(value, ">>"))
        return 1;
    if (!strcmp(value, ">&"))
        return 1;
    if (!strcmp(value, "<&"))
        return 1;
    if (!strcmp(value, ">|"))
        return 1;
    if (!strcmp(value, "<>"))
        return 1;
    return 0;
}
*/

enum status gr_redir(struct ast **ast)
{
    struct ast *redir_ast = init_ast(AST_REDIR, NULL);
    struct token *token = tok_peek();
    if (tok_peek()->type == IO_NUMBER)
    {
        tok_pop();
        struct ast *optional = init_ast(AST_TOKEN, token);
        redir_ast = add_child(redir_ast, optional);
    }
    token = tok_peek();
    CHECK_GOTO(token->type != CHEVRON, error);
    tok_pop();
    struct ast *chevron = init_ast(AST_TOKEN, token);
    redir_ast = add_child(redir_ast, chevron);
    token = tok_peek();
    CHECK_GOTO(IS_WORDABLE(token), error);
    struct ast *new_ast = init_ast(AST_TOKEN, token);
    tok_pop();
    redir_ast = add_child(redir_ast, new_ast);
    *ast = add_child(*ast, redir_ast);
    return OK;
error:
    destroy_ast(redir_ast);
    return ERROR;
}
