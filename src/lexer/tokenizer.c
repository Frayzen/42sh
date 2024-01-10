#include "tokenizer.h"
struct token *g_cur = NULL;



struct token *next_token(char *str)
{
    
    struct token* result = g_cur;
    if (is_terminating(result))
    {
        g_cur = NULL;
        return result;
    }
    printf("createing\n");
    char *lexeme = str;//finder function
    struct token *next = init_token(lexeme);
    g_cur = next;
    return result;
}



