#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "tools/var_dict/var_dict.h"

struct var_dict *get_dict(void)
{
    static struct var_dict *var_dico = NULL;
    if (!var_dico)
        var_dico = calloc(1, sizeof(struct var_dict));
    return var_dico;
}

void dict_push(char * key, char * value)
{
    assert(strcmp(key, value));
    if (VAR_DICT->nb_entries == VAR_DICT_SIZE)
        exit_gracefully(VAR_DICO_FULL);
    VAR_DICT->entries[VAR_DICT->nb_entries].key = key;
    VAR_DICT->entries[VAR_DICT->nb_entries].value = value;
    VAR_DICT->nb_entries++;
}

char * dict_peek_new(char * key)
{
    for (int i = 0; i < VAR_DICT->nb_entries; i++)
    {
        if (VAR_DICT->entries[i].key == key)
        {
            return VAR_DICT->entries[i].value;
        }
    }
    return NULL;
}

char * dict_peek_old(char * value)
{
    for (int i = 0; i < VAR_DICT->nb_entries; i++)
    {
        if (VAR_DICT->entries[i].value == value)
            return VAR_DICT->entries[i].key;
    }
    return NULL;
}

void dict_pop(char * key)
{
    int index = -1;
    for (int i = 0; i < VAR_DICT->nb_entries; ++i)
    {
        if (VAR_DICT->entries[i].key == key)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    VAR_DICT->entries[index] = VAR_DICT->entries[VAR_DICT->nb_entries - 1];
    VAR_DICT->nb_entries--;
}

void dict_free(void)
{
    if (VAR_DICT)
        free(VAR_DICT->entries);
}
/*
void dict_print(void)
{
    if (!VAR_DICT->nb_entries)
        return;
    prchar *f("┏━━━━━━━━━━━━━┓\n");
    for (char * i = 0; i < VAR_DICT->nb_entries; i++)
    {
        prchar *f("┃ %d", VAR_DICT->entries[i].key);
        if (VAR_DICT->entries[i].key < 1000)
            prchar *f(" ");
        if (VAR_DICT->entries[i].key < 100)
            prchar *f(" ");
        if (VAR_DICT->entries[i].key < 10)
            prchar *f(" ");
        prchar *f(" ↦ ");
        if (VAR_DICT->entries[i].value < 1000)
            prchar *f(" ");
        if (VAR_DICT->entries[i].value < 100)
            prchar *f(" ");
        if (VAR_DICT->entries[i].value < 10)
            prchar *f(" ");
        prchar *f("%d ┃\n", VAR_DICT->entries[i].value);
    }
    prchar *f("┗━━━━━━━━━━━━━┛\n");
}
*/
