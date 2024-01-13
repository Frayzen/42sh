#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"



enum status gr_if(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->type != IF)
        return ERROR;
    tok_pop();
    struct ast *if_ast = init_ast(AST_IF, NULL);
    
    if (gr_compound_list(&if_ast) == ERROR)
        goto error;
     
    if (tok_peek()->type != THEN)
        goto error;
    tok_pop();
    
    if (gr_compound_list(&if_ast) == ERROR)
        goto error;
    
    if (tok_peek()->type == ELSE || tok_peek()->type == ELIF)
    {

        printf("enter else\n");
        if(gr_else(&if_ast) == ERROR)
            goto error;
    }
    
    printf("tok below\n");
    print_token(token);
    if (tok_peek()->type != FI)
        goto error;
    tok_pop(); 
    printf("heeeee if\n");
    *ast = add_child(*ast, if_ast);
    return OK;

error:
    destroy_ast(if_ast);
    goto error;
}

