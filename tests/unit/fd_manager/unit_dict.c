#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>

#include "tools/fd_manager/fd_manager.h"

Test(fd_dict, empty_dict)
{
    cr_assert_eq(DICT->nb_entries, 0);
}

Test(fd_dict, all)
{
    for (int i = 0; i < 10; i++)
        dict_push(i, 10 + i);
    cr_assert_eq(DICT->nb_entries, 10);
    for (int i = 0; i < DICT->nb_entries; i++)
    {
        cr_assert_eq(dict_peek_new(i), i + 10);
        cr_assert_eq(dict_peek_old(i + 10), i);
    }
    cr_assert_eq(dict_peek_old(1), -1);
    cr_assert_eq(dict_peek_new(10), -1);
    int nb_entries_init = DICT->nb_entries;
    for (int i = 0; i < nb_entries_init; i++)
    {
        dict_pop(i);
        cr_assert_eq(DICT->nb_entries, nb_entries_init - (i + 1));
    }
    dict_free();
}
