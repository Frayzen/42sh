#include <stdlib.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

void add_to_item_list(struct ast_for *ast)
{
    struct token *item = tok_peek();
    // tok_pop();
    ast->item_list =
        realloc(ast->item_list, ++ast->nb_items * sizeof(struct exp_str *));
    ast->item_list[ast->nb_items - 1] = item->str;
    tok_pop();
}

/* rule_for ='for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 * {'\n'} 'do' compound_list 'done';
 */
enum status gr_for(struct ast_list *ast)
{
    GR_DBG_START(For);
    if (tok_peek()->type != FOR)
        return ERROR;
    tok_pop_clean();
    if (!IS_WORDABLE(tok_peek()))
        return ERROR;
    struct ast_for *ast_for = init_ast(AST_FOR);
    ast_for->name = tok_peek()->str->value;
    tok_peek()->str->value = NULL;
    tok_pop_clean();
    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();
    else
    {
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        if (tok_peek()->type == IN)
        {
            tok_pop_clean();
            while (IS_WORDABLE(tok_peek()))
                add_to_item_list(ast_for);
            if (tok_peek()->type != SEMI_COLON)
                CHECK_GOTO(tok_peek()->type != NEWLINE, error);
            tok_pop_clean();
        }
    }
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    CHECK_GOTO(tok_peek()->type != DO, error);
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(ast_for)) == ERROR)
        goto error;
    CHECK_GOTO(tok_peek()->type != DONE, error);
    tok_pop_clean();
    add_child(ast, AST(ast_for));
    GR_DBG_RET(OK);
error:
    destroy_ast(AST_FOR(ast_for));
    GR_DBG_RET(ERROR);
}
