#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

/*
simple_command =
prefix { prefix }
{ prefix } WORD { element }
;
*/
enum status gr_simple_command(struct ast **ast)
{
    struct ast *ast_cmd = init_ast(AST_COMMAND, NULL);
    // {prefix}
    int nb_prefix = 0;
    while (gr_prefix(&ast_cmd) != ERROR)
        nb_prefix++;

    struct token *tok_word = tok_peek();
    if (!IS_COMMAND(tok_word) && nb_prefix == 0)
        goto error;
    // WORLD
    struct ast *word = init_ast(AST_TOKEN, tok_word);
    tok_pop();
    ast_cmd = add_child(ast_cmd, word);

    // {element}
    while (gr_element(&ast_cmd) != ERROR)
        continue;

    *ast = add_child(*ast, ast_cmd);
    return OK;
error:
    destroy_ast(ast_cmd);

    return ERROR;
}
