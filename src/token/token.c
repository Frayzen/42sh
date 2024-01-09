#include "token.h"


char *g_elements[9] = {
    [IF] = "if" ,
    [THEN] = "then",
    [ELIF] = "elif",
    [ELSE] = "else",
    [FI] = "fi",
    [SEMI_COLON] = ";",
    [NEWLINE] = "\n",
    [QUOTE] = "'",
    [WORD] = NULL
};

int get_type(char *value)
{
    size_t len = strlen(value);
    int i = 0;
    while (g_elements[i] && !strcmp(g_elements[i], value))
        i++;
    return i;
}



struct token *init_token(char *value)
{
    struct token *tok = malloc(sizeof(struct token));   
    tok->type = get_type(value);
    tok->value = value;
    return tok;
}

void destroy_token(struct token *token)
{
    if (!token)
        return;
    if (token->value)
        free(token->value);
    free(token);
}


void print_token(struct token *token)
{
    if (!token)
        printf(" |NULL| ");
    else
        printf(" |%s|%d| ", token->value, token->type);
}


char *create_string(char *str)
{
    size_t len = strlen(str);
    // printf("len = %ld\n", len);
    char * res = malloc(sizeof(char) * len);
    res = strncpy(res, str, len);
    res[len] = '\0';
    return res;

}
