#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
/*
pipeline = ['!'] command { '|' {'\n'} command } ;
*/
enum status gr_pipeline(struct ast_list *list)
{
    GR_DBG_START(Pipeline);
    struct ast_pipe *pipe = init_ast(AST_PIPE);
    struct token *token = tok_peek();
    if (token->type == NEGATION)
    {
        pipe->negated = true;
        tok_pop_clean();
    }
    CHECK_GOTO(gr_command(pipe) == ERROR, error);
    while (tok_peek()->type == PIPE)
    {
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        CHECK_GOTO(gr_command(pipe), error);
    }
    add_child(list, AST(pipe));
    GR_DBG_RET(OK);
error:
    destroy_ast(pipe);
    GR_DBG_RET(ERROR);
}
