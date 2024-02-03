#ifndef VARS_H
#define VARS_H
#include <stdbool.h>

#define _POSIX_C_SOURCE 200809L
#define DEFAULT_IFS " \t\n"

/***
 * set the variable named name to the value
 * @param name a string following the rules of the scl
 * @param value that is already expanded
 */
void assign_var(char *name, char *value);

/***
 * Setup the environment variable required to make the shell function
 */
void init_env_vars(void);

/***
 * retrieve a value from the env variables
 * @param name the name of the variable
 * returns the string value allocated
 */
char *retrieve_var(char *name);

/***
 * Return a read only string of the content of the env variable
 * @param name the name of the var
 * @return NULL if the var doesnt exist, the read only content otherwise
 */
char *read_var(char *name);

/*
 * Unset the variable
 * @param name the name of the var
 * @return true if the variable what set
 */
int unset_var(char *name);

/*
 * Check for the existence of the variable
 * @param name the name of the var
 * @return true if the variable exists
 */
int is_set_var(char *name);

/***
 * Export the variable
 * @param name the name of the variable to export
 */
void export_var(char *name);

/***
 * Export all the variable that are not
 */
void export_all(void);

#endif /* !VARS_H */
