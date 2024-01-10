#include "token.h"


char *g_elements[10] = {
    [IF] = "if" ,
    [THEN] = "then",
    [ELIF] = "elif",
    [ELSE] = "else",
    [FI] = "fi",
    [SEMI_COLON] = ";",
    [NEWLINE] = "\n",
    [QUOTE] = "'",
    [BSZERO] = "\0",
    [WORD] = NULL
};

//
int is_terminating(struct token *token)
{
    if(!token)
        return 0;
    return token->type < 2;
}


int get_type(char *value)
{
    int i = 0;
    while (g_elements[i] && strcmp(g_elements[i], value))
        i++;
    return i;
}


void set_term_value(struct token *token)
{
    free(token->value);
    token->value = malloc(sizeof(char) * 5);
    token->value = strcpy(token->value, "term");
}


struct token *init_token(char *value)
{
    struct token *tok = malloc(sizeof(struct token));   
    tok->type = get_type(value);
    tok->value = value;
    if (is_terminating(tok))
        set_term_value(tok);
    // printf("here\n");
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
    printf("\n");
}


char *create_string(char *str)
{
    if (str == NULL)
        return NULL;
    size_t len = strlen(str);
    char * res = malloc(sizeof(char) * len + 1);
    res = strncpy(res, str, len);
    res[len] = '\0';
    return res;

}
