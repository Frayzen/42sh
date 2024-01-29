#ifndef BUILTINS_H
#define BUILTINS_H

/***
 * Execute the echo builtin given the arguments
 * @param argv the agument list, null terminated
 */
void builtin_echo(char **argv);

/***
 * Execute the exit builtin given the arguments
 * @param argv the agument list, null terminated
 */
void builtin_exit(char **argv);

/***
 * Execute the cd builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_cd(char **argv);

/***
 * Execute the exit builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_unset(char **argv);

/***
 * Execute the dot builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_dot(char **argv);

/***
 * Execute the continue builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_continue(char **argv);

/***
 * Execute the break builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_break(char **argv);

/* Execute the exit builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_unset(char **argv);

#endif /* !BUILTINS_H */
