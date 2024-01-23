#include "assignment.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "parser/command/expander.h"
#include "str/string.h"
// manage the variable assignments using environ to store, restore and update

struct lex_str *extract_value(struct lex_str *str, size_t eq_pos)
{
    size_t offset = eq_pos + 1;
    size_t val_size = str->size - offset;
    struct lex_str *val = calloc(1, sizeof(struct lex_str));
    val->value = strdup(str->value + offset);
    val->expand = calloc(1, sizeof(char) * val_size);
    memcpy(val->expand, str->expand + offset, val_size);
    val->size = val_size;
    return val;
}

struct assignment *init_assignment(struct lex_str *str)
{
    char *eq = strchr(str->value, '=');
    if (!eq)
        return NULL;
    struct assignment *ass = calloc(1, sizeof(struct assignment));
    size_t eq_pos = eq - str->value;

    str->value[eq_pos] = '\0';
    ass->name = strdup(str->value);

    struct lex_str *value_str = extract_value(str, eq_pos);
    exp_register_str(&ass->exp, value_str);
    return ass;
}

void destroy_assignment(struct assignment *assignment)
{
    if (!assignment)
        return;
    if (assignment->name)
        free(assignment->name);
    clean_expansion(&assignment->exp);
    if (assignment->prev)
        free(assignment->prev);
    free(assignment);
}

void ass_list_append(struct assignment_list *assign_list,
                     struct assignment *element)
{
    assign_list->ass_list = realloc(assign_list->ass_list, ++assign_list->size);
    assign_list->ass_list[assign_list->size - 1] = element;
}

void clean_assignments(struct assignment_list *assign_list)
{
    if (!assign_list)
        return;
    for (size_t i = 0; i < assign_list->size; i++)
    {
        struct assignment *a = assign_list->ass_list[i];
        destroy_assignment(a);
    }
    free(assign_list->ass_list);
}

void apply_assignments(struct assignment_list *asslist)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        char *val = NULL;
        assert(expand_next(ass->exp.head, &val) == NULL);
        assert(val != NULL);
        ass->prev = assign_var(ass->name, val);
        free(val);
    }
}

void revert_assignments(struct assignment_list *asslist)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        free(assign_var(ass->name, ass->prev));
    }
}

