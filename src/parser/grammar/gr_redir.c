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
|[IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD ;
*/
enum status gr_redir(struct ast_redir *ast)
{
    GR_START(Redir);
    struct token *token = tok_peek();
    int io_number = -1;
    if (tok_peek()->type == IO_NUMBER)
    {
        io_number = atoi(token->str->value);
        tok_pop_clean();
    }
    token = tok_peek();
    if (token->type != CHEVRON)
        GR_RET(io_number == -1 ? NO_MATCH : ERROR);
    struct redirection *redir = calloc(1, sizeof(struct redirection));
    redir->type = get_redir_type(token->str->value);
    redir->io_number = io_number;
    tok_pop_clean();
    token = tok_peek();
    if (!IS_WORDABLE(token))
    {
        free(redir);
        GR_RET(ERROR);
    }
    exp_register_str(&redir->exp, token->str);
    tok_pop();
    append_redir(ast, redir);
    GR_RET(OK);
}
