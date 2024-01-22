
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "tools/str/string.h"
#include "vars.h"

#include <stdlib.h>
#include <string.h>
#define NAME true
#define VALUE false

char *split_ass_word(char *ass_word, bool name)
{
    char *equalSign = strchr(ass_word, '=');
    if (equalSign == NULL)
        return NULL;
    if (name)
    {
        size_t len = equalSign - ass_word;
        char *ret = strdup(ass_word);
        ret[len] = '\0';
        return ret;
    }
    char *ret = strdup(equalSign + 1);
    return ret;

    
}

void append_ass(struct ass_list *ass_list, struct exp_str *ass_word)
{
    ass_list->size++;
    ass_list->elements =
        realloc(ass_list->elements, ass_list->size * sizeof(struct ass_list));
    struct ass_element *element = calloc(1, sizeof(struct ass_element));
    element->ass_word = strdup(ass_word->value);
    element->prev = retrieve_var(ass_word->value);
    ass_list->elements[ass_list->size -1] = element;
    destroy_exp_str(ass_word);
}



void assign_vars(struct ass_list ass_list)
{
    for(size_t i = 0; i < ass_list.size; i++)
    {
        char *value = split_ass_word(ass_list.elements[i]->ass_word, VALUE);
        char *name = split_ass_word(ass_list.elements[i]->ass_word, NAME);
        printf("setting |%s|   to |%s|\n", name, value);
        setenv(name, value, true);
        free(value);
        free(name);
    }
}
//
// void assign_vars(struct ass_list ass_list)
// {
//     for(size_t i = 0; i < ass_list.size; i++)
//     {
//         char *value = split_ass_word(ass_list.elements[i]->ass_word, VALUE);
//         char *name = split_ass_word(ass_list.elements[i]->ass_word, NAME);
//         setenv(name, value, true);
//         free(value);
//         free(name);
//     }
// }


void print_ass_element(struct ass_element *elm)
{
    printf("prev = %s\n", elm->prev);
    printf("cur = %s\n", elm->ass_word);

}
void print_ass_list(struct ass_list *ass_list)
{
    for (size_t i = 0; i < ass_list->size; i++)
        print_ass_element(ass_list->elements[i]);
}

char *retrieve_var(char *pair)
{
    const char *equalSign = strchr(pair, '=');
    if (equalSign == NULL)
        return NULL;
    size_t nameLength = equalSign - pair;
    char *name = (char *)malloc(nameLength + 1);
    strncpy(name, pair, nameLength);
    name[nameLength] = '\0';
    char *value = getenv(name);
    free(name);
    return value;
}

void destroy_ass_list(struct ass_list * ass_list)
{
    for (size_t i = 0; i < ass_list->size; i++)
    {
        if (ass_list->elements[i]->ass_word)
            free(ass_list->elements[i]->ass_word);
        if (ass_list->elements[i]->prev)
            free(ass_list->elements[i]->prev);
        free(ass_list->elements[i]);
    }
    free(ass_list->elements);
}
