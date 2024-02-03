
#include "tools/funct_dict/funct_dict.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"

static struct funct_dictionary dict;

void funct_dict_push(char *name, struct ast_sh *body)
{
    if (dict.nb_entries == FUNCT_DICT_SIZE)
        exit_gracefully(FUNCT_DICO_FULL);
    dict.entries[dict.nb_entries].name = name;
    dict.entries[dict.nb_entries].body = body;
    dict.nb_entries++;
}

struct ast_sh *funct_dict_peek_value(char *name)
{
    for (int i = 0; i < dict.nb_entries; i++)
    {
        if (!strcmp(dict.entries[i].name, name))
        {
            return dict.entries[i].body;
        }
    }
    return NULL;
}

char *funct_dict_peek_key(struct ast_sh *body)
{
    for (int i = 0; i < dict.nb_entries; i++)
    {
        if (dict.entries[i].body == body)
            return dict.entries[i].name;
    }
    return NULL;
}

void funct_dict_pop(char *name)
{
    int index = -1;
    for (int i = 0; i < dict.nb_entries; ++i)
    {
        if (!strcmp(dict.entries[i].name, name))
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    if (index != dict.nb_entries - 1)
        dict.entries[index] = dict.entries[dict.nb_entries - 1];
    dict.nb_entries--;
}

void funct_dict_free(void)
{
    for (int i = 0; i < dict.nb_entries; i++)
    {
        free(dict.entries[i].name);
        destroy_ast(dict.entries[i].body);
    }
}
