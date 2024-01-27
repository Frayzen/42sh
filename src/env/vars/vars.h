#ifndef VARS_H
#define VARS_H
#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L

/*
 * set the variable named name to the value
 * @param name a string following the rules of the scl
 * @param value that is already expanded
 * @return the string value of the variable or null if the variable has not been
 * assigned
 */
char *assign_var(char *name, char *value);

/*
 * retrieve a value from the env variables and allocate it
 * @param name the name of the variable
 * returns the string value allocated
 */
char *retrieve_var(char *name);

/*
 * Unset the variable
 * @param name the name of the var
 * @return true if the variable what set
 */
bool unset_var(char *name);

#endif /* !VARS_H */
