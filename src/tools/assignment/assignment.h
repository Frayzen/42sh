#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "env/vars/vars.h"
#include "tools/str/string.h"
#include "parser/command/expander.h"
struct assignment
{
    char *name;
    struct expansion *exp;
    char *prev;
};

struct assignment_list
{
    size_t size;
    struct assignment **ass_list;
};

/*
 * initializes an assignment struct and
 * sets the previous value if any otherwise null
 * @param str lexeme associated to the token
 * returns the allocated assignment structure
 */
struct assignment *init_assignment(struct lex_str *str);

/*
 * destroys the assignment structure
 * @param assignment structure to destory
 */
void destroy_assignment(struct assignment *assignment);

/*
 * appends an assignment structure to the list of assignements
 * @param assign_list the list to append token
 * @param ass the element to append
 */
void ass_list_append(struct assignment_list * assing_list, 
struct assignment *ass);

#endif // !ASSIGNMENT_H
