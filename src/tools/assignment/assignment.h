#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env/vars/vars.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "tools/str/string.h"
struct assignment
{
    char *name;
    struct expansion exp;
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
void append_ass_list(struct assignment_list *assing_list,
                     struct assignment *ass);

/***
 * Free all assignments from the assignment list
 * @param assign_list the assignment list
 */
void clean_assignments(struct assignment_list *assign_list);

/***
 * Revert the assignments made by apply_assignments
 * @param asslist the list of assignment
 */
void revert_assignments(struct assignment_list *asslist);
/***
 * Apply the assignments contained in apply_assignments
 * @param asslist the list of assignment
 */
void apply_assignments(struct assignment_list *asslist);

#endif // !ASSIGNMENT_H
