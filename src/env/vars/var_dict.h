#ifndef VAR_DICT_H
#define VAR_DICT_H

#include "tools/definitions.h"

// Double linked list
struct sh_var
{
    char *name;
    char *value;
    BOOL exported;
};

struct sh_varlist
{
    struct sh_var var;
    struct sh_varlist *next;
    struct sh_varlist *prev;
};

/***
 * Retrieve the variable by its name
 * @param name the name of the variable
 * @return the sh_var struct associated or NULL if does not exist
 */
struct sh_var *get_var(char *name);

/***
 * Retrieve the variable by its name or create it if it doesnt exist
 * @param name the name of the variable
 * @return the sh_var struct associated
 */
struct sh_var *get_or_create_var(char *name);

/***
 * Remove the variable from the list of variable by its name
 * @param name the name of the variable
 * @return true if the removal happened
 */
BOOL remove_var(char *name);

/***
 * Clear all of the variables from the dictionnary
 */
void clean_variables(void);

/***
 * Retrieve the begining of the list
 * @return the begining of the linked list
 */
struct sh_varlist *get_varlist(void);

#endif /* !VAR_DICT_H */
