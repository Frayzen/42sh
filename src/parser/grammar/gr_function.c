#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "lexer/token_saver.h"
#include "tools/token/token.h"
/*
funcdec = WORD '(' ')' '{\n}' shell_command
*/
enum status gr_function(struct ast_pipe *pipe)
{
    GR_DBG_START(Command);
    struct token *token = tok_peek();
    if (!IS_WORDABLE(token))
        return ERROR;
    tok_pop_clean();
    if (tok_peek()->type != PRTH_OPEN)
        return ERROR;
    tok_pop_clean();
    if (tok_peek()->type != PRTH_CLOSED)
        return ERROR;
    tok_pop_clean();
    while (is_terminal(tok_peek()))
      tok_pop_clean();
    while (gr_shell_cmd(arbre) == OK)
    ;
    GR_DBG_RET(OK);
}
