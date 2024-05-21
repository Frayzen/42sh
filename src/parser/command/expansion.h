#ifndef EXPANSION_H
#define EXPANSION_H
#include <stdbool.h>
#include <stddef.h>

#include "tools/str/string.h"

struct expandable
{
    struct expandable *next;
    // If the next element is separated from the current
    bool link_next;
    // either an ast (sub command) or a char (other cases)
    void *content;
    enum expand_type type;
};

struct expansion
{
    size_t size;
    struct expandable *head;
    struct expandable *tail;
};

/***
 * Initialize the expansion
 * @return the expansion
 */
struct expansion *expansion_init(void);

/***
 * Push the expandable to back of the expansion items
 * @param exp the expansion
 * @return true if everything went fine
 */
void expansion_push_back(struct expansion *exp, struct expandable *item);

/***
 * Create and expandable from parameters
 * @param content the value of the expandable
 * @param type either VAR STR or SUB_CMD
 * @param link_next if the next argument is separated form the current
 * @return the new expandable
 */
struct expandable *expandable_init(void *content, enum expand_type type,
                                   bool link_next);
/***
 * Print expansion
 * @param exp the expansion
 */
void expansion_print(const struct expansion *exp);
/***
 * Free all of the elements of the expansion (but not the expansion itslef)
 * @param expansion the expansion
 */
void clean_expansion(struct expansion *list);

#endif /* !EXPANSION_H */
