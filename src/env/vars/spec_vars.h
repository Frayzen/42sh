#ifndef SPEC_VARS_H
#define SPEC_VARS_H

/*
 * returns all the arguments to the script as an array
 */
char **get_argv(void);
/*
 * returns all the argument of the script as one string
 */

/*
 * returns the last exit status of the last command
 */
char *get_argstr(void);

/*
 * returns the pid of the current shell
 */
char *get_pid(void);

/*
 * returns the positional argument
 */
char *get_pos_arg(char *id);

/*
 * returns the arg count
 */
char *get_argc(void);

/*
 * returns  a random integer between 0 and 32767
 */
char *get_rand(void);

/*
 * returns the user id of the current user
 */
char *get_uid(void);
#endif // !SPEC_VARS_H
