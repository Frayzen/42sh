#ifndef ARG_LIST_H
#define ARG_LIST_H
#include <stdbool.h>
#include <stddef.h>

enum arg_type
{
    VAR,
    STR,
};

struct arg
{
    struct arg *next;
    struct arg *prev;
    // If the next element is separated from the current
    bool link_next;
    char *content;
    enum arg_type type;
};

struct arglist
{
    size_t size;
    struct arg *head;
    struct arg *tail;
};

/***
 * Initialize the arglist
 * @return the arglist
 */
struct arglist *arglist_init(void);

/***
 * Push the value to the back of the list
 * @param list the list
 * @param value the string of the arg
 * @param arg_type either VAR or STR
 * @param follow_next if the next argument is separated form the current
 */
int arglist_push_back(struct arglist *list, bool link_nex, char *content,
                       enum arg_type type);
void arglist_print(const struct arglist *list);
int arglist_push_front(struct arglist *list, bool link_nex, char *content,
                       enum arg_type type);
void arglist_clear(struct arglist *list);
#endif /* !ARG_LIST_H */
