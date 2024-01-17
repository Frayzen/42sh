#include "tools/fd_manager/fd_manager.h"

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>

Test(fd_dict, null_dict)
{
    cr_assert(!DICT);
}

Test(fd_dict, init)
{
    struct fd_dictionary *dict = dict_init();
    cr_assert(dict);
    cr_assert_eq(dict->nb_entries, 0);
    free(dict->entries);
}

Test(fd_dict, all)
{
    dict_push(0, 10);
    cr_assert(DICT);
    cr_assert_eq(DICT->nb_entries, 1);
    dict_push(1, 11);
    dict_push(2, 12);
    dict_push(3, 13);
    dict_push(4, 14);
    dict_push(5, 15);
    dict_push(6, 16);
    dict_push(7, 17);
    dict_push(8, 18);
    dict_push(9, 19);
    cr_assert_eq(DICT->nb_entries, 10);
    for (int i = 0; i < DICT->nb_entries; i++)
    {
        cr_assert_eq(dict_peek_new(i), i + 10);
        cr_assert_eq(dict_peek_old(i + 10), i);
    }
    int nb_entries_init = DICT->nb_entries;
    for (int i = 0; i < nb_entries_init; i++)
    {
        dict_pop(i);
        cr_assert_eq(DICT->nb_entries, nb_entries_init - (i + 1));
    }
    dict_free();
}
