#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "exit/exit.h"
#include "tools/fd_manager/fd_manager.h"

struct fd_dictionary *get_dict(void)
{
    static struct fd_dictionary *fd_dico = NULL;
    if (!fd_dico)
        fd_dico = calloc(1, sizeof(struct fd_dictionary));
    return fd_dico;
}

void dict_push(int old_fd, int new_fd)
{
    assert(old_fd != new_fd);
    assert(old_fd >= 0 && new_fd >= 0);
    assert(old_fd < 1024 && new_fd < 1024);
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
    DICT->entries[index] = DICT->entries[DICT->nb_entries - 1];
    DICT->nb_entries--;
}

void dict_free(void)
{
    if (DICT)
        free(DICT->entries);
}

void dict_print(void)
{
    if (!DICT->nb_entries)
        return;
    printf("┏━━━━━━━━━━━━━┓\n");
    for (int i = 0; i < DICT->nb_entries; i++)
    {
        printf("┃ %d", DICT->entries[i].old_fd);
        if (DICT->entries[i].old_fd < 1000)
            printf(" ");
        if (DICT->entries[i].old_fd < 100)
            printf(" ");
        if (DICT->entries[i].old_fd < 10)
            printf(" ");
        printf(" ↦ ");
        if (DICT->entries[i].new_fd < 1000)
            printf(" ");
        if (DICT->entries[i].new_fd < 100)
            printf(" ");
        if (DICT->entries[i].new_fd < 10)
            printf(" ");
        printf("%d ┃\n", DICT->entries[i].new_fd);
    }
    printf("┗━━━━━━━━━━━━━┛\n");
}
