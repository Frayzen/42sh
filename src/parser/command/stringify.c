#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env/vars/specials.h"
#include "env/vars/vars.h"
#include "expander.h"

// Find the first delimiter
static int is_in(char c, char *delims)
{
    int i = 0;
    while (delims[i] && delims[i] != c)
        i++;
    return delims[i] == c;
}

// Stops at each delimiter
static char *my_str_tok(char *buf, char *delims)
{
    static char *str = NULL;
    if (buf)
        str = buf;
    if (!str)
        return NULL;
    int i = 0;
    char *b = str;
    bool not_delim_found = false;
    // Go to the next delimiter that is after a not delimiter and not a space
    // Also increase b for every leading space
    while (str[i])
    {
        if (is_in(str[i], delims))
        {
            if (!not_delim_found && isspace(str[i]))
                b++;
            else
                break;
        }
        else
            not_delim_found = true;
        i++;
    }
    while (str[i])
    {
        if (is_in(str[i], delims))
            break;
        i++;
    }
    if (!str[i])
        str = NULL;
    else
    {
        str[i] = '\0';
        str += i + 1;
    }
    return b;
}

// Expand the unquoted var (= expand it and split it by ' ')
// return the new current or NULL if the expansion is empty
static struct expandable *expand_unquoted_var(struct expandable *cur)
{
    assert(cur->type == UNQUOTED_VAR);
    char *val = retrieve_var(cur->content);
    if (val[0] == '\0')
    {
        free(val);
        return NULL;
    }
    char *ifs = DEFAULT_IFS;
    if (is_set_var("IFS"))
        ifs = read_var("IFS");
    char *elem = my_str_tok(val, ifs);
    struct expandable *last = NULL;
    struct expandable *first = NULL;
    if (!elem)
    {
        last = expandable_init(val, STR_LITTERAL, cur->link_next);
        last->next = cur->next;
        free(val);
        return last;
    }
    while (elem)
    {
        struct expandable *new_string =
            expandable_init(strdup(elem), STR_LITTERAL, false);
        if (last)
            last->next = new_string;
        else
            first = new_string;
        last = new_string;
        elem = my_str_tok(NULL, ifs);
    }
    last->link_next = cur->link_next;
    last->next = cur->next;
    free(val);
    return first;
}

// Expand the quoted var (= expand it)
// return the new current or NULL if the expansion is empty
static struct expandable *expand_quoted_var(struct expandable *cur)
{
    assert(cur->type == QUOTED_VAR);
    char *content = retrieve_var(cur->content);
    if (content[0] == '\0')
    {
        free(content);
        return NULL;
    }
    struct expandable *new_string =
        expandable_init(content, STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

// Expand the string litteral (= duplicate it)
static struct expandable *expand_str(struct expandable *cur)
{
    assert(IS_STR_TYPE(cur->type));
    struct expandable *new_string =
        expandable_init(strdup(cur->content), STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

// This function calls the appropriate subfunction in order to create a string
// litteral linked list from the currrent expandable
struct expandable *stringify_expandable(struct expandable *cur)
{
    struct expandable *exp = expand_special_var(cur);
    if (exp)
        return exp;
    switch (cur->type)
    {
    case QUOTED_VAR:
        return expand_quoted_var(cur);
    case UNQUOTED_VAR:
        return expand_unquoted_var(cur);
    default:
        return expand_str(cur);
    }
}
