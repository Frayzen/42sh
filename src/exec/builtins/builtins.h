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
 * Execute the exit builtin given the arguments
 * @param argv the agument list, null terminated
 */
int builtin_unset(char **argv);

#endif /* !BUILTINS_H */
