#include "gr_tools.h"

#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"

#define GET_STATE (set_state(NULL))
#define GR_DBG_SPACE "|    "
struct gr_state *set_state(struct gr_state *new)
{
    static struct gr_state *last = NULL;
    if (new != NULL)
        last = new;
    return last;
}

int gr_debug_id(void)
{
    static int id = 1;
    return id++;
}

void gr_debug_start(char *name)
{
    if (get_env_flag()->debug_grammar)
    {
        struct gr_state *state = calloc(1, sizeof(struct gr_state));
        state->prev = GET_STATE;
        state->id = gr_debug_id();
        state->name = name;
        if (GET_STATE == NULL)
            state->depth = 0;
        else
            state->depth = GET_STATE->depth + 1;
        set_state(state);
        for (int i = 0; i < state->depth; i++)
            printf(GR_DBG_SPACE);
        printf("%s\n", state->name);
    }
}

enum status gr_debug_end(enum status status)
{
    if (get_env_flag()->debug_grammar)
    {
        struct gr_state *state = GET_STATE;
        set_state(state->prev);
        for (int i = 0; i < state->depth; i++)
            printf(GR_DBG_SPACE);
        static const char *status_lt[] = {
            [OK] = "OK",
            [ERROR] = "ERROR",
            [NO_MATCH] = "NO_MATCH",
        };
        printf("<- %s [%s]\n", status_lt[status], state->name);
        free(state);
    }
    return status;
}
