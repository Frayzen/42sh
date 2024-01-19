#ifndef FINDER_H
#define FINDER_H

#include <stdbool.h>
#include <stddef.h>

#define IS_BLANK(Pending) ((Pending)->blank)

struct string
{
    char *value;
    size_t size;
};

struct pending
{
    struct string str;
    bool force_word;
    bool backslashed;
    bool blank;
    bool error;
};

/***
 * finder: gets character by character and return the string token
 * @return a string struct or NULL on error
 * @see struct string
 */
const struct pending *finder(void);
#endif /* FINDER_H */
