#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"
// and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
enum status gr_and_or(struct ast_list *list)
{
    struct ast_and_or *ast_ao = init_ast(AST_AND_OR);
    CHECK_GOTO(gr_pipeline(AST_LIST(&ast_ao->base)) == ERROR, error);
    while (tok_peek()->type == AND || tok_peek()->type == OR)
    {
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        // TODO modify so it adds to list
        CHECK_GOTO(gr_pipeline(AST_LIST(&ast_ao->list->next)) == ERROR, error);
    }
    add_child(list, AST(ast_ao));
    return OK;
error:
    destroy_ast(AST(ast_ao));
    return ERROR;
}
