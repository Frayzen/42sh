#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"
/*
funcdec = WORD '(' ')' '{\n}' shell_command
*/
enum status gr_function(struct ast_list *ast)
{
    GR_DBG_START(Function);
    struct ast_funct *ast_funct = AST_FUNCT(ast);
    struct token *token = tok_peek();
    CHECK_GOTO(!IS_WORDABLE(token), error);
    ast_funct->name = token;
    tok_pop();
    CHECK_GOTO(tok_peek()->type != PRTH_OPEN, error);
    tok_pop_clean();
    CHECK_GOTO(tok_peek()->type != PRTH_CLOSED, error);
    tok_pop_clean();
    while (tok_peek()->terminal)
        tok_pop_clean();
    CHECK_GOTO(gr_shell_cmd(AST_LIST(&ast_funct->body)) == ERROR, error);
    // don't know if &ast->body, does it keep the result here ?
    // TODO put into dictionnary IN THE EXEC PART, NOT HERE
    add_child(ast, AST(ast_funct));
    GR_DBG_RET(OK);
error:
    destroy_ast(ast_funct);
    return ERROR;
}
