#include <assert.h>
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

void append_ao_type(struct ast_and_or *ao, enum token_type type)
{
    assert(type == AND || type == OR);
    ao->types =
        realloc(ao->types, sizeof(enum token_type) * AST_LIST(ao)->nb_children);
    ao->types[AST_LIST(ao)->nb_children - 1] = type;
}

// and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
enum status gr_and_or(struct ast_list *list)
{
    GR_DBG_START(AndOr);
    struct ast_and_or *ast_ao = init_ast(AST_AND_OR);
    CHECK_GOTO(gr_pipeline(AST_LIST(ast_ao)) == ERROR, error);
    struct token *tok = tok_peek();
    while (tok->type == AND || tok->type == OR)
    {
        append_ao_type(ast_ao, tok->type);
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        CHECK_GOTO(gr_pipeline(AST_LIST(ast_ao)) == ERROR, error);
        tok = tok_peek();
    }
    add_child(list, AST(ast_ao));
    return OK;
error:
    destroy_ast(AST(ast_ao));
    return ERROR;
}
