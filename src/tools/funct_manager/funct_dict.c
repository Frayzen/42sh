#include "tools/funct_manager/funct_dict.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "exit/error_handler.h"

struct funct_dictionary *get_funct_dict(void)
{
    static struct funct_dictionary *funct_dico = NULL;
    if (!funct_dico)
        funct_dico = calloc(1, sizeof(struct funct_dictionary));
    return funct_dico;
}

void funct_dict_push(char *name, struct ast *body)
{
    if (FUNCT_DICT->nb_entries == FUNCT_DICT_SIZE)
        exit_gracefully(FUNCT_DICO_FULL);
    FUNCT_DICT->entries[FUNCT_DICT->nb_entries].name = name;
    FUNCT_DICT->entries[FUNCT_DICT->nb_entries].body = body;
    FUNCT_DICT->nb_entries++;
}

struct ast *funct_dict_peek_value(char *name)
{
    for (int i = 0; i < FUNCT_DICT->nb_entries; i++)
    {
        if (FUNCT_DICT->entries[i].name == name)
        {
            return FUNCT_DICT->entries[i].body;
        }
    }
    return NULL;
}

char *funct_dict_peek_key(struct ast *body)
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
    FUNCT_DICT->entries[index] =
        FUNCT_DICT->entries[FUNCT_DICT->nb_entries - 1];
    FUNCT_DICT->nb_entries--;
}

void funct_dict_free(void)
{
    if (FUNCT_DICT)
        free(FUNCT_DICT->entries);
}

void funct_dict_print(void)
{
    if (!FUNCT_DICT->nb_entries)
        return;
    printf("┏━━━━━━━━━━━━━┓\n");
    for (int i = 0; i < FUNCT_DICT->nb_entries; i++)
    {
        printf("┃ %d", FUNCT_DICT->entries[i].name);
        if (FUNCT_DICT->entries[i].name < 1000)
            printf(" ");
        if (FUNCT_DICT->entries[i].name < 100)
            printf(" ");
        if (FUNCT_DICT->entries[i].name < 10)
            printf(" ");
        printf(" ↦ ");
        if (FUNCT_DICT->entries[i].body < 1000)
            printf(" ");
        if (FUNCT_DICT->entries[i].body < 100)
            printf(" ");
        if (FUNCT_DICT->entries[i].body < 10)
            printf(" ");
        printf("%d ┃\n", FUNCT_DICT->entries[i].body);
    }
    printf("┗━━━━━━━━━━━━━┛\n");
}
