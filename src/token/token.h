#ifndef TOKEN_H
#define TOKEN_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
enum token_type
{
    NEWLINE,//keep newline at index 1 for is_terminating_char
    BSZERO, //keep bakcslah zero at index 1 for is_terminating_char
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMI_COLON,
    QUOTE,
    WORD,
    TERMINAL        
};

struct token
{
    enum token_type type;
    char *value;
};

/***
* creates a token, sets value to lexime or 'term', sets type 
* @param value <- the lexeme passed from the finder
* @return <- a malloced token structure
*/
struct token *init_token(char *value);


/***
* frees a token
* @param token<- the token to free
*/
void destroy_token(struct token *token);

/***
* prints the token value and type number
* @param token <- token to print 
*/
void print_token(struct token *token);

/***
* creates an allocated tring from str
* @param str <- the string to create 
* @return <- a malloced string
*/
char *create_string(char *str);

/***
* checks if the current token is a terminating character 
* @param token -< the toekn to check
* @return <- 1 if is it terminatin, 0 if null or non terminating
*/
int is_terminating(struct token *token);



#endif // !TOKEN_H
