
#include "tools/funct_dict/funct_dict.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"

struct funct_dictionary *get_funct_dict(void)
{
    static struct funct_dictionary *funct_dico = NULL;
    if (!funct_dico)
        funct_dico = calloc(1, sizeof(struct funct_dictionary));
    return funct_dico;
}

void funct_dict_push(char *name, struct ast_sh *body)
{
    struct funct_dictionary *dict = FUNCT_DICT;
    dict->entries = realloc(dict->entries,
                            sizeof(struct funct_pair) * (dict->nb_entries + 1));
    dict->entries[FUNCT_DICT->nb_entries].name = name;
    dict->entries[FUNCT_DICT->nb_entries].body = body;
    dict->nb_entries++;
}

struct ast_sh *funct_dict_peek_value(char *name)
{
    for (int i = 0; i < FUNCT_DICT->nb_entries; i++)
    {
        if (!strcmp(FUNCT_DICT->entries[i].name, name))
        {
            return FUNCT_DICT->entries[i].body;
        }
    }
    return NULL;
}

char *funct_dict_peek_key(struct ast_sh *body)
{
    for (int i = 0; i < FUNCT_DICT->nb_entries; i++)
    {
        if (FUNCT_DICT->entries[i].body == body)
            return FUNCT_DICT->entries[i].name;
    }
    return NULL;
}

void funct_dict_pop(char *name)
{
    int index = -1;
    for (int i = 0; i < FUNCT_DICT->nb_entries; ++i)
    {
        if (FUNCT_DICT->entries[i].name == name)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    struct funct_pair to_rm = FUNCT_DICT->entries[index];
    for (int i = index; i < FUNCT_DICT->nb_entries - 1; i++)
    {
        FUNCT_DICT->entries[i] = FUNCT_DICT->entries[i + 1];
    }
    free(to_rm.name);
    destroy_ast(to_rm.body);
    FUNCT_DICT->nb_entries--;
}

void funct_dict_free(void)
{
    if (!FUNCT_DICT)
        return;
    for (int i = 0; i < FUNCT_DICT->nb_entries; i++)
    {
        free(FUNCT_DICT->entries[i].name);
        destroy_ast(FUNCT_DICT->entries[i].body);
    }
    free(FUNCT_DICT->entries);
}
