#ifndef FINDER_H
#define FINDER_H

#include <stdbool.h>

#include "tools/str/string.h"

#define IS_BLANK(Pending) ((Pending)->blank)
struct pending
{
    struct string str;
    bool force_word;
    bool backslashed;
    bool blank;
};

/***
 * finder: gets character by character and return the string token
 * @return a string struct
 * @see struct string
 */
const struct pending *finder(void);
#endif /* FINDER_H */
