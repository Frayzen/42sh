#include "string.h"

bool is_number(char *val)
{
    int i = 0;
    while (val[i])
    {
        if (val[i] > '9' || val[i] < '0')
            return false;
        i++;
    }
    return true;
}
