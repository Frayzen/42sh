#include "rules.h"

struct ast *gr_entry(void)
{
    // default case, should be at the very end
    return gr_element();
}
