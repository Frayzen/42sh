#ifndef FINDER_H
#define FINDER_H
#include "../io_backend/backend_saver.h"

/***
 * finder: gets character by character until the word is recognizable, or t here
 * @get_backend function: to look at the next character without popping it
 * @pop_backend function: pops the character in backend so the next one is
 * available
 * @return the string to be used for the token
 */
char *finder(void);

enum type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMI_COLON,
    BACK_N,
    SINGLE_QUOTE,
    ECHO,
    T_TRUE,
    T_FALSE,
    MAX_TYPE,
    WORD,
};

#endif /* FINDER_H */
