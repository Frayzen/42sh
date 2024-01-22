#define _POSIX_C_SOURCE  200809L
#include "vars.h"
#include <stdlib.h>
#include <string.h>

void append_ass(struct ass_list *ass_list, struct exp_str *ass_word)
{
    print_exp_str(ass_word);
    ass_list->elements = realloc(ass_list->elements, ass_list->size * sizeof(struct ass_list));
    struct ass_element *element = calloc(1, sizeof(struct ass_element));
    element->ass_word = strdup(ass_word->value);
    element->prev = retrieve_var(ass_word->value);
    ass_list->elements[ass_list->size++] = element;
    destroy_exp_str(ass_word);
}
//
// void set_var(char *name, char *value);
//
//

    


char *retrieve_var(char *pair)
{
    const char *equalSign = strchr(pair, '=');
    if (equalSign == NULL) {
        return NULL;
    }
    size_t nameLength = equalSign - pair;
    char *name = (char *)malloc(nameLength + 1);
    if (name == NULL) {
        return NULL;
    }
    strncpy(name, pair, nameLength);
    name[nameLength] = '\0';
    char *value = getenv(name);
    free(name);
    return value;
}

