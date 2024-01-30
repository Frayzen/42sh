#define _POSIX_C_SOURCE 200809L
#include "var_dict.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static struct sh_varlist *list = NULL;

static struct sh_varlist *get_varlist(char *name)
{
    if (!list)
        return NULL;
    struct sh_varlist *cur = list;
    while (cur->next != list)
    {
        if (!strcmp(name, cur->var.name))
            return cur;
        cur = cur->next;
    }
    return NULL;
}

struct sh_var *get_or_create_var(char *name)
{
    struct sh_var *var = get_var(name);
    if (var)
        return var;
    var = calloc(1, sizeof(struct sh_var));
    var->name = strdup(name);
    var->value = strdup("");
    var->exported = false;
    struct sh_varlist *nl = calloc(1, sizeof(struct sh_varlist));
    nl->next = list;
    if (list)
    {
        nl->prev = list->prev;
    }
    else
    {
        nl->prev = nl;
        nl->next = nl;
    };
    nl->next->prev = nl;
    nl->prev->next = nl;
    return var;
}

struct sh_var *get_var(char *name)
{
    struct sh_varlist *varlist = get_varlist(name);
    if (!varlist)
        return NULL;
    return &varlist->var;
}

BOOL remove_var(char *name)
{
    struct sh_varlist *varlist = get_varlist(name);
    if (!varlist)
        return false;
    free(varlist->var.value); 
    if (varlist->next == varlist)
        list = NULL;
    else
    {
        varlist->next->prev = varlist->prev;
        varlist->prev->next = varlist->next;
    }
    return true;
}

void clean_variables(void)
{
    while (list)
        remove_var(list->var.name);
}
