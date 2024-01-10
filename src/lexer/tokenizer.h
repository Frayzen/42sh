#ifndef LEXER_H
#define LEXER_H
#include "../token/token.h"

/***
*asks for the next token from the lexeme finder which is stored in g_cur
which is initially null, needs to be called once before the parse function in main,
* @param str <- for testing purposes without finder function, will be void after 
* @return <- the current token stored in g_cur, and sets g_cur to the next token
*/
struct token *next_token(char *str);


#endif // !LEXER_H
