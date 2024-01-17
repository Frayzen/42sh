#include <stdio.h>
#include <stdlib.h>

#include "exit/exit.h"
#include "tools/fd_manager/fd_manager.h"

struct fd_dictionary *dict_init(void)
{
    struct fd_dictionary *fd_dico = malloc(sizeof(struct fd_dictionary));
    fd_dico->nb_entries = 0;
    return fd_dico;
}

struct fd_dictionary *get_dict(struct fd_dictionary *dico)
{
    static struct fd_dictionary *fd_dico = NULL;
    if (dico)
        fd_dico = dico;
    return fd_dico;
}

void dict_push(int old_fd, int new_fd)
{
    if (!DICT)
        get_dict(dict_init());
    if (DICT->nb_entries == DICT_SIZE)
        exit_gracefully(FD_DICO_FULL);
    DICT->entries[DICT->nb_entries].old_fd = old_fd;
    DICT->entries[DICT->nb_entries].new_fd = new_fd;
    DICT->nb_entries++;
}

int dict_peek_new(int old_fd)
{
    for (int i = 0; i < DICT->nb_entries; i++)
    {
        if (DICT->entries[i].old_fd == old_fd)
        {
            return DICT->entries[i].new_fd;
        }
    }
    return -1;
}

int dict_peek_old(int new_fd)
{
    for (int i = 0; i < DICT->nb_entries; i++)
    {
        if (DICT->entries[i].new_fd == new_fd)
            return DICT->entries[i].old_fd;
    }
    return -1;
}

void dict_pop(int old_fd)
{
    int index = -1;
    for (int i = 0; i < DICT->nb_entries; ++i)
    {
        if (DICT->entries[i].old_fd == old_fd)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    for (int i = index; i < DICT->nb_entries - 1; ++i)
    {
        DICT->entries[i] = DICT->entries[i + 1];
    }
    DICT->nb_entries--;
}

void dict_free(void)
{
    if (!DICT)
    {
        free(DICT->entries);
    }
}
