#ifndef VARS_H
#define VARS_H
#include <stddef.h>
#include "tools/str/string.h"
struct ass_element
{
    char *prev;
    char *ass_word;
};
struct ass_list
{
   struct ass_element **elements;
   size_t size;
};
void append_ass(struct ass_list *ass_list, struct exp_str *ass_word);
void set_var(char *name, char *value);
void print_ass_element(struct ass_element *elm);

void print_ass_list(struct ass_list *ass_list);
char *retrieve_var(char *name);
void destroy_ass_list(struct ass_list * ass_list);
void assign_vars(struct ass_list ass_list);


#endif /* !VARS_H */
