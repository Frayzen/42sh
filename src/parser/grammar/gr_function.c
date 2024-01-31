#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <fnmatch.h>
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

// check that the name of the function is valid according to SCL
static int check_name_valid(char *name)
{
    int i = 0;
    while (name[i] != 0)
    {
        if (i == 0)
        {
            if (!isalpha(name[i]) && name[i] != '_')
                return 0;
        }
        else if (!isalnum(name[i]) && name[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

static int check_not_builtin_name(char *name)
{
  const char **built_names = toktype_lookup();
  for (int i = T_TRUE; i < WORD; i++)
  {
    if (!strcmp(name, built_names[i]))
      return 0;
  }
  return 1;
}

enum status gr_function(struct ast_list *ast)
{
    GR_DBG_START(Function);
    struct ast_funct *ast_funct = init_ast(AST_FUNCT);
    struct token *token = tok_peek();
    struct token *token2 = tok_peek2();
    CHECK_GOTO(!IS_WORDABLE(token) || !check_name_valid(token->str->value) || !check_not_builtin_name(token->str->value) || token2->type != PRTH_OPEN,
               error);
    ast_funct->name = strdup(token->str->value);
    tok_pop_clean();
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
