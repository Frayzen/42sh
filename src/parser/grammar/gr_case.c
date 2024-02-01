#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "parser/command/expander.h"
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
        GR_RET(ERROR);
    ast->list_cond[ast->nb_cond - 1] = malloc(sizeof(char *));
    ast->list_cond[ast->nb_cond - 1][0] = calloc(1, sizeof(struct expansion));
    exp_register_str(ast->list_cond[ast->nb_cond - 1][0], tok_peek()->str);
    tok_pop();

    int i = 1;
    while (tok_peek()->type == PIPE)
    {
        tok_pop_clean();

        if (!IS_WORDABLE(tok_peek()))
            GR_RET(ERROR);
        ast->list_cond[ast->nb_cond - 1] =
            realloc(ast->list_cond[ast->nb_cond - 1], sizeof(char *) * (i + 1));
        ast->list_cond[ast->nb_cond - 1][i] =
            calloc(1, sizeof(struct expansion));
        exp_register_str(ast->list_cond[ast->nb_cond - 1][i], tok_peek()->str);
        tok_pop();
        i++;
    }

    if (tok_peek()->type != PRTH_CLOSED)
        GR_RET(ERROR);
    tok_pop_clean();

    struct ast_list *list_cmd = init_ast(AST_LIST);
    ast->cmds = realloc(ast->cmds, sizeof(struct list *) * ast->nb_cond);
    if (gr_compound_list(list_cmd) == ERROR)
        ast->cmds[ast->nb_cond - 1] = NULL;
    else
        ast->cmds[ast->nb_cond - 1] = list_cmd;

    ast->list_cond[ast->nb_cond - 1] =
        realloc(ast->list_cond[ast->nb_cond - 1], sizeof(char *) * (i + 1));
    ast->list_cond[ast->nb_cond - 1][i] = NULL;

    GR_RET(OK);
}

// case_clause = case_item { ';;' {'\n'} case_item } [';;'] {'\n'} ;

enum status gr_case_clause(struct ast_case *ast)
{
    if (tok_peek()->type == ESAC)
        GR_RET(OK);
    ast->nb_cond++;
    ast->list_cond = realloc(ast->list_cond, sizeof(char *) * ast->nb_cond);
    if (gr_case_item(ast) == ERROR)
        GR_RET(ERROR);

    while (tok_peek()->type == DBL_SEMI_COLON)
    {
        tok_pop_clean();
        if (tok_peek()->type == ESAC)
            break;

        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();

        ast->nb_cond++;
        ast->list_cond = realloc(ast->list_cond, sizeof(char *) * ast->nb_cond);
        if (gr_case_item(ast) == ERROR)
            GR_RET(ERROR);
    }

    if (tok_peek()->type == DBL_SEMI_COLON)
        tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    GR_RET(OK);
}

// rule_case = 'case' WORD {'\n'} 'in' {'\n'} [case_clause] 'esac' ;

enum status gr_case(struct ast_sh *sh)
{
    GR_START(Case);
    if (tok_peek()->type != CASE)
        GR_RET(ERROR);
    tok_pop_clean();

    if (!IS_WORDABLE(tok_peek()))
        GR_RET(ERROR);
    struct ast_case *case_ast = init_ast(AST_CASE);
    exp_register_str(&case_ast->name, tok_peek()->str);
    tok_pop();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    if (tok_peek()->type != IN)
        GR_RET(ERROR);
    tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    if (gr_case_clause(case_ast) == ERROR)
        GR_RET_CLEAN(ERROR, AST(case_ast));

    if (tok_peek()->type != ESAC)
        GR_RET(ERROR);

    tok_pop_clean();
    sh->sh_cmd = AST(case_ast);
    GR_RET(OK);
}
