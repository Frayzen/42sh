#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "tools/str/string.h"
#include "env/vars/vars.h"
struct assignment
{
    char *name;
    struct lex_str *value;
    char *prev;
};

/*
 * initializes a assignment struct and sets the previous value if any 
 * @param str lexeme assiciated to the token, will be destroyed
 * returns the allocated assignment structure
 */
struct assignment *init_assignment(struct lex_str  *str);

/*
 * destroys the assignment structure
 * @param asstignment structure to destory
 */
void destroy_assignment(struct assignment *assignment);


#endif // !ASSIGNMENT_H
