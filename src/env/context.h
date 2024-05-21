#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
struct context
{
    FILE *saved_file;
    char saved_char;
    struct token *saved_token;
    struct token *saved_token2;
    struct ast **ast_root;
};

/***
 * Create a new context and return the current one
 * /!\ The file descriptor is unloaded as well, do not forget to call a function
 * from the io backend to set up the new streamer
 * @param file, the file to read from
 * @return the current context
 */
struct context *new_context(void);

/***
 * Load the context as the current one
 * Discard and free the ast root, token saved and char saved of the current
 * @param c the context to be loaded
 */
void load_context(struct context *c);

#endif /* !CONTEXT_H */
