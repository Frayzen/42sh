#ifndef SPECIALS_H
#define SPECIALS_H

#include <stdbool.h>

#define CHECK_ARGS(arginfo)                                             \
    if (!(arginfo) || (arginfo)->argc == 0)                                                                  \
    {                                                                          \
        return NULL;                                                     \
    }


enum var_type
{
    CLASSIC_VAR,
    SPECIAL_PARAMS, // $@
    PARAMS, // $*
    LAST_RET, // $?
    POSITIONAL_PARAM, // $n
    NB_PARAM, // $#
    RANDOM, // $RANDOM
    UID, // $UID
};

/***
 * Apply the special variable if any, return null if the variable is not known
 * @param cur, the current expandable
 * @return NULL if the variable is not special, the stringified expandable
 * otherwise
 */
struct expandable *expand_special_var(struct expandable *cur);

/***
 * Set the ret_val for the special variable $?
 * @param ret_val the new value of $?
 */
void set_ret_val(int ret_val);

#endif /* !SPECIALS_H */
