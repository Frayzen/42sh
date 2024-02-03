#define _POSIX_C_SOURCE 200809L

#include "specials.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg_saver/arg_saver.h"
#include "parser/command/expansion.h"
#include "tools/str/string.h"

static int is_arg_index(char *name)
{
    assert(name != NULL);
    if (*name == '0')
        return FALSE_B;
    while (*name)
    {
        if (*name > '9' || *name < '0')
            return FALSE_B;
        name++;
    }
    return TRUE_B;
}

static enum var_type get_var_type(char *name)
{
    if (is_arg_index(name))
        return POSITIONAL_PARAM;
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

struct expandable *build_argv(struct arg_info *arg_info, struct expandable *cur)
{
    if (arg_info->argc == 0)
        return NULL;
    struct expandable *ret = expandable_init(strdup(arg_info->argv[0]),
                                             STR_LITTERAL, cur->link_next);
    int i = 1;
    struct expandable *current = ret;
    while (i < arg_info->argc)
    {
        struct expandable *next = expandable_init(
            strdup(arg_info->argv[i]), STR_LITTERAL, current->link_next);
        current->next = next;
        current = next;
        i++;
    }
    return ret;
}

struct expandable *expand_special_var(struct expandable *cur, int *valid)
{
    *valid = FALSE_B;
    if (!IS_VAR_TYPE(cur->type))
        return NULL;
    *valid = TRUE_B;
    enum var_type type = get_var_type(cur->content);
    struct arg_info *arg_info = get_arg_info();
    switch (type)
    {
    case RANDOM:
        return expandable_init(itoa(rand() % 32768), STR_LITTERAL,
                               cur->link_next);
    case LAST_RET:
        return expandable_init(itoa(last_ret_val), STR_LITTERAL,
                               cur->link_next);
    case UID:
        return expandable_init(itoa(getuid()), STR_LITTERAL, cur->link_next);
    case NB_PARAM:
        return expandable_init(itoa(arg_info->argc), STR_LITTERAL,
                               cur->link_next);
    case POSITIONAL_PARAM: {
        int pos = atoi(cur->content);
        if (arg_info->argc < pos)
            return NULL;
        return expandable_init(strdup(arg_info->argv[pos - 1]), STR_LITTERAL,
                               cur->link_next);
    }
    case SPECIAL_PARAMS: {
        return build_argv(arg_info, cur);
    }
    case PARAMS:
        return expandable_init(strdup(arg_info->argstr), STR_LITTERAL,
                               cur->link_next);
    default:
        *valid = FALSE_B;
        return NULL;
    }
}
