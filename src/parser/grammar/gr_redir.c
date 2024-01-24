#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/redirection/redirection.h"
#include "tools/token/token.h"
/*
redirection =
|[IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
|WORD ;
*/
enum status gr_redir(struct ast_redir *ast)
{
    GR_DBG_START(Redir);
    struct redirection *redir = calloc(1, sizeof(struct redirection));

    struct token *token = tok_peek();
    if (tok_peek()->type == IO_NUMBER)
    {
        redir->io_number = atoi(token->str->value);
        tok_pop_clean();
    }
    else
        redir->io_number = -1;

    token = tok_peek();
    CHECK_GOTO(token->type != CHEVRON, error);
    redir->type = get_redir_type(token->str->value);
    tok_pop_clean();

    token = tok_peek();
    CHECK_GOTO(!IS_WORDABLE(token), error);
    exp_register_str(&redir->exp, token->str);
    tok_pop();

    append_redir(ast, redir);
    GR_DBG_RET(OK);
error:
    free(redir);
    GR_DBG_RET(ERROR);
}
