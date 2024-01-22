#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
int check_compound_token(enum token_type expected_type1,
                         enum token_type expected_type2, struct ast_list *list)
{
    struct token *token = tok_peek();
    if (token->type == expected_type1)
    {
        tok_pop_clean();
        if (gr_list(&list) == OK)
        {
            token = tok_peek();
            if (token->type != expected_type2)
                return 0;
            tok_pop_clean();
            return 1;
        }
        else
            return 0;
    }
    return 2;
}

enum status gr_shell_cmd(struct ast_list *list)
{
    GR_DBG_START(ShellCmd);

    int res = check_compound_token(BRACKET_OPEN, BRACKET_CLOSED, list);
    if (res == 1 || res == 0)
        return (res == 1) ? gr_debug_end(OK) : gr_debug_end(ERROR);
    res = check_compound_token(PARENTHESE_OPEN, PARENTHESE_CLOSED, list);
    if (res == 1 || res == 0)
        return (res == 1) ? gr_debug_end(OK) : gr_debug_end(ERROR);
    if (gr_if(list) == OK)
        GR_DBG_RET(OK);
    if (gr_while(list) == OK)
        GR_DBG_RET(OK);
    if (gr_until(list) == OK)
        GR_DBG_RET(OK);
    if (gr_for(list) == OK)
        GR_DBG_RET(OK);
    GR_DBG_RET(ERROR);
}
