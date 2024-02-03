#define _POSIX_C_SOURCE 200809L
#include "var_dict.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct sh_varlist *list = NULL;

struct sh_varlist *get_varlist(void)
{
    return list;
}

static struct sh_varlist *get_varlist_item(char *name)
{
    if (!list)
        return NULL;
    struct sh_varlist *cur = list;
    do
    {
        if (!strcmp(name, cur->var.name))
            return cur;
        cur = cur->next;
    } while (cur != list);
    return NULL;
}

struct sh_var *get_or_create_var(char *name)
{
    struct sh_var *var = get_var(name);
    if (var)
        return var;
    struct sh_varlist *nl = calloc(1, sizeof(struct sh_varlist));
    var = &(nl->var);
    var->name = strdup(name);
    var->value = NULL;
    var->exported = FALSE_B;
    nl->next = list;
    if (list)
        nl->prev = list->prev;
    else
    {
        nl->prev = nl;
        nl->next = nl;
    };
    nl->next->prev = nl;
    nl->prev->next = nl;
    list = nl;
    return var;
}

struct sh_var *get_var(char *name)
{
    struct sh_varlist *varlist = get_varlist_item(name);
    if (!varlist)
        return NULL;
    return &(varlist->var);
}

int remove_var(char *name)
{
    struct sh_varlist *varlist = get_varlist_item(name);
    if (!varlist)
        return FALSE_B;
    if (varlist->next == varlist)
        list = NULL;
    else
    {
        varlist->next->prev = varlist->prev;
        varlist->prev->next = varlist->next;
        list = varlist->next;
    }
    free(varlist->var.value);
    free(varlist->var.name);
    free(varlist);
    return TRUE_B;
}

void clean_variables(void)
{
    while (list)
        remove_var(list->var.name);
}
