#include "rules.h"

struct ast *gr_list(void)
{
    return gr_and_or();
}
