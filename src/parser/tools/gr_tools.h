#ifndef GR_TOOLS_H
#define GR_TOOLS_H

#include "grammar/rules.h"
struct gr_state{
    struct gr_state *prev;
    char *name;
    int id;
    int depth;
};

/***
 * A debug tool that need to be set at the begin of all grammar function
 * @param id the identifier of the function
 * @param name the name of the function
 */
void gr_debug_start(char *name);

/***
 * A debug tool that need to be set at the end of all grammar function
 * @param status the function return value
 * @return the same status for debug purposes
 */
enum status gr_debug_end(enum status status);

#define GR_DBG_START(GrRule) gr_debug_start(#GrRule);
#define GR_DBG_RET(Value) return gr_debug_end(Value);

#endif /* !GR_TOOLS_H */

