#include "gr_tools.h"

#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"

#define GR_DBG_SPACE "|    "

static struct gr_state *last = NULL;

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
        state->prev = last;
        state->id = gr_debug_id();
        state->name = name;
        if (last == NULL)
            state->depth = 0;
        else
            state->depth = last->depth + 1;
        last = state;
        for (int i = 0; i < state->depth; i++)
            printf(GR_DBG_SPACE);
        printf("%s\n", state->name);
    }
}

enum status gr_debug_end(enum status status)
{
    if (get_env_flag()->debug_grammar)
    {
        struct gr_state *state = last;
        last = state->prev;
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
