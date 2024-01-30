#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"
/*
funcdec = WORD '(' ')' '{\n}' shell_command
*/
enum status gr_function(struct ast_list *ast)
{
    GR_DBG_START(Function);
    struct ast_funct *ast_funct = init_ast(AST_FUNCT);
    struct token *token = tok_peek();
    struct token *token2 = tok_peek2();
    CHECK_GOTO(!IS_WORDABLE(token) || token2->type != PRTH_OPEN, error);
    ast_funct->name = strdup(token->str->value);
    tok_pop_clean();
    CHECK_GOTO(tok_peek()->type != PRTH_CLOSED, error);
    tok_pop_clean();

    while (tok_peek()->terminal)
        tok_pop_clean();

    CHECK_GOTO(gr_shell_cmd(AST_LIST(ast_funct)) == ERROR, error);
    add_child(ast, AST(ast_funct));
    GR_DBG_RET(OK);
error:
    destroy_ast(ast_funct);
    GR_DBG_RET(ERROR);
}
