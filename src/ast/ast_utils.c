#include "ast_utils.h"
#include <stddef.h>
#include <stdlib.h>


struct ast *add_child(struct ast* ast,struct token *token)
{
  struct ast *new_ast = init_ast(token);
  if (!ast)
    return new_ast;
  ast->children = realloc(ast->children, nb_children + 1);
  ast->children[nb_children] = new_ast;
  ast->nb_children++;
  return ast;
}


