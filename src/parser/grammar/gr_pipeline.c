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
    GR_START(Pipeline);
    struct ast_pipe *pipe = init_ast(AST_PIPE);
    struct token *token = tok_peek();
    int consumed = FALSE_B;
    while (token->type == NEGATION)
    {
        consumed = TRUE_B;
        pipe->negated = !pipe->negated;
        tok_pop_clean();
        token = tok_peek();
    }
    switch (gr_command(pipe))
    {
    case NO_MATCH:
        GR_RET_CLEAN(consumed ? ERROR : NO_MATCH, pipe);
    case ERROR:
        GR_RET_CLEAN(ERROR, pipe);
    case OK:
        break;
    }
    while (tok_peek()->type == PIPE)
    {
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        if (gr_command(pipe) != OK)
            GR_RET_CLEAN(ERROR, pipe);
    }
    add_child(list, AST(pipe));
    GR_RET(OK);
}
