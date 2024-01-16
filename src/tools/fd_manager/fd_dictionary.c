#include <stdlib.h>

#include "exit/exit.h"
#include "tools/fd_manager/fd_manager.h"

struct fd_dictionary *init_fd_dictionary(void)
{
    struct fd_dictionary *fd_dico = malloc(sizeof(struct fd_dictionary));
    fd_dico->nb_entries = 0;
    return fd_dico;
}

struct fd_dictionary *get_fd_dictionary(struct fd_dictionary *dico)
{
    static struct fd_dictionary *fd_dico = NULL;
    if (dico)
        fd_dico = dico;
    return fd_dico;
}

void add_to_fd_dico(int old_fd, int new_fd)
{
    if (!FD_DICTIONARY)
        get_fd_dictionary(init_fd_dictionary());
    if (FD_DICTIONARY->nb_entries == DICTIONARY_SIZE)
        exit_gracefully(FD_DICO_FULL);
    FD_DICTIONARY->entries[FD_DICTIONARY->nb_entries].old_fd = old_fd;
    FD_DICTIONARY->entries[FD_DICTIONARY->nb_entries].new_fd = new_fd;
    FD_DICTIONARY->nb_entries++;
}

int get_new_fd(int old_fd)
{
    for (int i = 0; i < FD_DICTIONARY->nb_entries; i++)
    {
        if (FD_DICTIONARY->entries[i].old_fd == old_fd)
            return FD_DICTIONARY->entries[i].new_fd;
    }
    return -1;
}

int get_old_fd(int new_fd)
{
    for (int i = 0; i < FD_DICTIONARY->nb_entries; i++)
    {
        if (FD_DICTIONARY->entries[i].new_fd == new_fd)
            return FD_DICTIONARY->entries[i].old_fd;
    }
    return -1;
}

void remove_form_fd_dictionary(int old_fd)
{
    int index = -1;
    for (int i = 0; i < FD_DICTIONARY->nb_entries; ++i)
    {
        if (FD_DICTIONARY->entries[i].old_fd == old_fd)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    for (int i = index; i < FD_DICTIONARY->nb_entries - 1; ++i)
    {
        FD_DICTIONARY->entries[i] = FD_DICTIONARY->entries[i + 1];
    }
    FD_DICTIONARY->nb_entries--;
}

void free_fd_dictionary(void)
{
    if (!FD_DICTIONARY)
    {
        free(FD_DICTIONARY->entries);
    }
}
