#include <stdlib.h>
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/redirection.h"
#include "tools/token/token.h"
/*
redirection =
|[IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
|WORD ;
*/
enum status gr_redir(struct ast_redir *ast)
{
    struct redirection* redir = calloc(1, sizeof(struct redirection));

    struct token *token = tok_peek();
    if (tok_peek()->type == IO_NUMBER)
    {
        redir->io_number = atoi(token->value);
        tok_pop();
    }else
        redir->io_number = -1;

    token = tok_peek();
    CHECK_GOTO(token->type != CHEVRON, error);
    redir->redir = token->value;
    tok_pop();

    token = tok_peek();
    CHECK_GOTO(!IS_WORDABLE(token), error);
    redir->to = token->value;
    tok_pop();

    append_redir(ast, redir);
    return OK;
error:
    free(redir);
    return ERROR;
}
