#ifndef VARS_H
#define VARS_H
#define _POSIX_C_SOURCE 200809L

/***
 * set the variable named name to the value
 * @param name a string following the rules of the scl
 * @param value that is already expanded
 * @return the string value of the variable or null if the variable has not been
 * assigned
 */
char *assign_var(char *name, char *value);

/***
 * Setup the environment variable required to make the shell function
 */
void setup_vars(void);

/***
 * retrieve a value from the env variables
 * @param name the name of the variable
 * returns the string value of the variable otherwise null
 */
char *retrieve_var(char *name);

#endif /* !VARS_H */
