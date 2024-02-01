#define _POSIX_C_SOURCE 200809L
#include "assignment.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "env/vars/vars.h"
#include "parser/command/expander.h"

// manage the variable assignments using environ to store, restore and update

struct lex_str *extract_value(struct lex_str *str, size_t eq_pos)
{
    size_t offset = eq_pos + 1;
    size_t val_size = str->size - offset;
    struct lex_str *val = calloc(1, sizeof(struct lex_str));
    val->value = strdup(str->value + offset);
    val->expand = calloc(val_size, sizeof(enum expand_type));
    memcpy(val->expand, str->expand + offset,
           sizeof(enum expand_type) * val_size);
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
    if (assignment->prev)
        free(assignment->prev);
    clean_expansion(&assignment->exp);
    free(assignment);
}

void append_ass_list(struct assignment_list *assign_list,
                     struct assignment *element)
{
    assign_list->ass_list =
        realloc(assign_list->ass_list,
                sizeof(struct assignment) * (++assign_list->size));
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

char *argv_to_str(char **argv)
{
    size_t totalLength = 0;
    int i = 0;
    while (argv[i] != NULL)
        totalLength += strlen(argv[i++]) + 1;
    char *result = (char *)malloc(totalLength);
    size_t currentPosition = 0;
    i = 0;
    while (argv[i] != NULL)
    {
        size_t argLength = strlen(argv[i]);
        memcpy(result + currentPosition, argv[i], argLength);
        currentPosition += argLength;
        if (argv[i + 1] != NULL)
            result[currentPosition++] = ' ';
        i++;
    }
    result[currentPosition] = '\0';
    return result;
}

void apply_assignments(struct assignment_list *asslist)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        char **argv = expand(&ass->exp);
        assert(argv);
        char *val = NULL;
        val = argv[1] ? argv_to_str(argv) : strdup(argv[0]);
        ass->prev = retrieve_var(ass->name);
        assign_var(ass->name, val);
        free(val);
        destroy_expanded(argv);
    }
}

void discard_assignments(struct assignment_list *asslist, bool revert)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        if (ass->prev)
        {
            if (revert)
                assign_var(ass->name, ass->prev);
            free(ass->prev);
            ass->prev = NULL;
        }
        else
            unset_var(ass->name);
    }
}
