#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

bool is_builtin(enum token_type type)
{
    switch (type)
    {
    case ECHO:
    case T_TRUE:
    case T_FALSE:
        return true;
    default:
        return false;
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
    GR_DBG_START(SimpleCommand);
    struct ast_cmd *cmd = init_ast(AST_CMD);
    // {prefix}
    int nb_prefix = 0;
    while (gr_prefix(cmd) != ERROR)
        nb_prefix++;
    struct token *tok_word = tok_peek();
    if (!IS_COMMAND(tok_word) && nb_prefix == 0)
        goto error;
    // WORLD
    cmd_register_token(cmd, tok_word);
    tok_pop();
    // {element}
    while (gr_element(cmd) != ERROR)
        continue;
    // {element}
    add_child(list, AST(cmd));
    GR_DBG_RET(OK);
error:
    destroy_ast(cmd);
    GR_DBG_RET(ERROR);
}
