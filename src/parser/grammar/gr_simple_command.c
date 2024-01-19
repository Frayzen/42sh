#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_utils.h"

/*
simple_command =
prefix { prefix }
{ prefix } WORD { element }
;
*/
enum status gr_simple_command(struct ast_list *list)
{
    struct ast_cmd *cmd = init_ast(AST_CMD);
    // {prefix}
    int nb_prefix = 0;
    while (gr_prefix(cmd) != ERROR)
        nb_prefix++;

    struct token *tok_word = tok_peek();
    if (!IS_COMMAND(tok_word) && nb_prefix == 0)
        goto error;
    // WORLD
    append_arg(cmd, tok_word->value);
    tok_pop();

    // {element}
    while (gr_element(cmd) != ERROR)
        continue;

    add_child(list, AST(cmd));
    return OK;
error:
    destroy_ast(cmd);
    return ERROR;
}
