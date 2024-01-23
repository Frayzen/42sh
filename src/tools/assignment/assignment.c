#include "assignment.h"

#include <stdlib.h>
#include <string.h>

#include "env/vars/vars.h"
#include "parser/command/expansion.h"
#include "str/string.h"
// manage the variable assignments using environ to store, restore and update

struct lex_str *extract_value(struct lex_str *str, size_t eq_pos)
{
    size_t offset = eq_pos + 1;
    size_t val_size = str->size - offset;

    struct lex_str *val = calloc(1, sizeof(struct lex_str));
    val->value = strdup(str->value + offset);

    val->expand = calloc(1, sizeof(char));
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

    ass->prev = retrieve_var(ass->name);

    struct lex_str *value_str = extract_value(str, eq_pos);
    ass->exp = expansion_init();
    exp_register_str(ass->exp, value_str);
    return ass;
}

void destroy_assignment(struct assignment *assignment)
{
    if (!assignment)
        return;
    if (assignment->name)
        free(assignment->name);
    if (assignment->exp)
        clean_expansion(assignment->exp);
    if (assignment->prev)
        free(assignment->prev);
    free(assignment);
}


void ass_list_append(struct assignment_list * assign_list,
                     struct assignment *element)
{
    assign_list->ass_list = realloc(assign_list->ass_list, ++assign_list->size);
    assign_list->ass_list[assign_list->size] = element;
}
