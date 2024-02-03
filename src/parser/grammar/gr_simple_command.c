#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

int is_builtin(enum token_type type)
{
    switch (type)
    {
    case ECHO:
    case T_TRUE_B:
    case T_FALSE_B:
        return TRUE_B;
    default:
        return FALSE_B;
    }
}

/*
simple_command =
prefix { prefix }
{ prefix } WORD { element }
;
*/
enum status gr_simple_command(struct ast_list *list)
{
    GR_START(SimpleCommand);
    struct ast_cmd *cmd = init_ast(AST_CMD);
    // {prefix}
    int consumed = FALSE_B;
    enum status st;
    do
    {
        st = gr_prefix(cmd);
        if (st == ERROR)
            GR_RET_CLEAN(ERROR, cmd);
        if (st == NO_MATCH)
            break;
        consumed = TRUE_B;
    } while (TRUE_B);
    struct token *tok_word = tok_peek();
    struct token *tok_word2 = tok_peek2();
    if (!IS_COMMAND(tok_word) || tok_word2->type == PRTH_OPEN)
    {
        if (!consumed)
            GR_RET_CLEAN(NO_MATCH, cmd);
        add_child(list, AST(cmd));
        GR_RET(OK);
    } // WORLD
    exp_register_str(&cmd->args_expansion, tok_word->str);
    tok_pop();
    // {element}
    do
        st = gr_element(cmd);
    while (st == OK);
    if (st == ERROR)
        GR_RET_CLEAN(ERROR, cmd);
    add_child(list, AST(cmd));
    GR_RET(OK);
}
