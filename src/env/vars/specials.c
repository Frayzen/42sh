#include <unistd.h>
#define _POSIX_C_SOURCE 200809L
#include "specials.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_saver/arg_saver.h"
#include "parser/command/expansion.h"
#include "tools/str/string.h"

static enum var_type get_var_type(char *name)
{
    if (!strcmp("@", name))
        return SPECIAL_PARAMS;
    if (!strcmp("*", name))
        return PARAMS;
    if (!strcmp("?", name))
        return LAST_RET;
    if (is_number(name))
        return POSITIONAL_PARAM;
    if (!strcmp("#", name))
        return NB_PARAM;
    if (!strcmp("RANDOM", name))
        return RANDOM;
    if (!strcmp("UID", name))
        return UID;
    return CLASSIC_VAR;
}

static char *itoa(int i)
{
    char *ret = calloc(15, sizeof(char));
    sprintf(ret, "%d", i);
    return ret;
}

static int last_ret_val = 0;

void set_ret_val(int ret_val)
{
    last_ret_val = ret_val;
}








struct expandable *expand_special_var(struct expandable *cur)
{
    if (!IS_VAR_TYPE(cur->type))
        return NULL;
    enum var_type type = get_var_type(cur->content);
    switch (type)
    {
    case RANDOM:
        return expandable_init(itoa(rand() % 32768), STR_LITTERAL,
                               cur->link_next);
    case LAST_RET:
        return expandable_init(itoa(last_ret_val), STR_LITTERAL,
                               cur->link_next);
    case UID: 
         return expandable_init(itoa(getuid()), STR_LITTERAL,
                               cur->link_next);
    case NB_PARAM: 
         return expandable_init(itoa(getuid()), STR_LITTERAL,
                               cur->link_next);
    case POSITIONAL_PARAM: 
         return expandable_init(itoa(getuid()), STR_LITTERAL,
                               cur->link_next);
    case PARAMS: 
         return expandable_init(itoa(getuid()), STR_LITTERAL,
                               cur->link_next);
    case SPECIAL_PARAMS: 
         return expandable_init(itoa(getuid()), STR_LITTERAL,
                               cur->link_next);

    default:
        return NULL;
    }
}
