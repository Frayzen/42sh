#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "parser/grammar/rules.h"
#include "parser/tools/gr_tools.h"
#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

// case_item = ['('] WORD { '|' WORD } ')' {'\n'} [compound_list] ;

enum status gr_case_item(struct ast_case *ast)
{
    if (tok_peek()->type == PRTH_OPEN)
        tok_pop_clean();

    if (!IS_WORDABLE(tok_peek()))
    {
        ast->nb_cond--;
        GR_RET(NO_MATCH);
    }
    ast->list_cond[ast->nb_cond - 1] = malloc(sizeof(char *));
    ast->list_cond[ast->nb_cond - 1][0] = calloc(1, sizeof(struct expansion));
    exp_register_str(ast->list_cond[ast->nb_cond - 1][0], tok_peek()->str);
    tok_pop();

    int i = 1;
    while (tok_peek()->type == PIPE)
    {
        tok_pop_clean();

        ast->list_cond[ast->nb_cond - 1] =
            realloc(ast->list_cond[ast->nb_cond - 1], sizeof(char *) * (i + 1));
        if (!IS_WORDABLE(tok_peek()))
        {
            ast->cmds =
                realloc(ast->cmds, sizeof(struct list *) * ast->nb_cond);
            ast->cmds[ast->nb_cond - 1] = NULL;
            ast->list_cond[ast->nb_cond - 1][i] = NULL;
            GR_RET(ERROR);
        }
        ast->list_cond[ast->nb_cond - 1][i] =
            calloc(1, sizeof(struct expansion));
        exp_register_str(ast->list_cond[ast->nb_cond - 1][i++],
                         tok_peek()->str);
        tok_pop();
    }

    ast->list_cond[ast->nb_cond - 1] =
        realloc(ast->list_cond[ast->nb_cond - 1], sizeof(char *) * (i + 1));
    ast->list_cond[ast->nb_cond - 1][i] = NULL;

    struct ast_list *list_cmd = init_ast(AST_LIST);
    ast->cmds = realloc(ast->cmds, sizeof(struct list *) * ast->nb_cond);
    ast->cmds[ast->nb_cond - 1] = NULL;

    if (tok_peek()->type != PRTH_CLOSED)
        GR_RET_CLEAN(ERROR, list_cmd);
    tok_pop_clean();

    if (gr_compound_list(list_cmd) == OK)
        ast->cmds[ast->nb_cond - 1] = list_cmd;

    GR_RET(OK);
}
