#include "lexer.h"
struct token *g_cur = NULL;



struct token *next_token(char *str)
{
    char *lexeme = create_string(str);//finder function
    
    struct token* result = g_cur;
    struct token *next = init_token(lexeme);
    g_cur = next;
    return result;
}



