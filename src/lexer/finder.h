#ifndef FINDER_H
#define FINDER_H

#include <stdbool.h>

#include "tools/str/string.h"

#define IS_BLANK(Pending) ((Pending)->blank)

struct pending
{
    struct exp_str str;
    bool force_word;
    bool backslashed;
    bool blank;
    bool in_quote;
};

/***
 * finder: gets character by character and return the string token
 * @return a string struct
 * @see struct exp_str
 */
struct pending *finder(void);
#endif /* FINDER_H */
