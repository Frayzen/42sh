#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "parser/tools/gr_tools.h"
#include "parser/tools/gr_utils.h"
#include "tools/token/token.h"

/* rule_for ='for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 * {'\n'} 'do' compound_list 'done';
 */
enum status gr_for(struct ast_list *ast)
{
    GR_START(For);
    if (tok_peek()->type != FOR)
        GR_RET(NO_MATCH);
    tok_pop_clean();
    if (!IS_WORDABLE(tok_peek()))
        GR_RET(ERROR);
    struct ast_for *ast_for = init_ast(AST_FOR);
    ast_for->name = strdup(tok_peek()->str->value);
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
            {
                exp_register_str(&ast_for->exp, tok_peek()->str);
                tok_pop();
            }
            if (tok_peek()->type != SEMI_COLON && tok_peek()->type != NEWLINE)
                GR_RET_CLEAN(ERROR, ast_for);
            tok_pop_clean();
        }
    }
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    if (tok_peek()->type != DO)
        GR_RET_CLEAN(ERROR, ast_for);
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(ast_for)) != OK)
        GR_RET_CLEAN(ERROR, ast_for);
    if (tok_peek()->type != DONE)
        GR_RET_CLEAN(ERROR, ast_for);
    tok_pop_clean();
    add_child(ast, AST(ast_for));
    GR_RET(OK);
}
