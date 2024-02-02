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
/*
funcdec = WORD '(' ')' '{\n}' shell_command
*/
enum status gr_function(struct ast_list *ast)
{
    GR_START(Function);
    struct token *token = tok_peek();
    struct token *token2 = tok_peek2();
    if (!((token)->type >= IF) || !check_name_valid(token->str->value)
        || token2->type != PRTH_OPEN)
        GR_RET(NO_MATCH);
    struct ast_funct *ast_funct = init_ast(AST_FUNCT);
    ast_funct->name = strdup(token->str->value);
    tok_pop_clean();
    tok_pop_clean();
    if (tok_peek()->type != PRTH_CLOSED)
        GR_RET_CLEAN(ERROR, ast_funct);

    tok_pop_clean();
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    
    if (gr_shell_cmd(AST(ast_funct)) != OK)
        GR_RET_CLEAN(ERROR, ast_funct);
    add_child(ast, AST(ast_funct));
    GR_RET(OK);
}
