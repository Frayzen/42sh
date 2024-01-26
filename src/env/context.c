

#include "context.h"

#include <stdio.h>
#include <stdlib.h>

#include "io_backend/backend_saver.h"
#include "io_backend/io_streamers.h"
#include "lexer/token_saver.h"
#include "tools/ast/ast.h"

/***
 * /!\ The file descriptor is unloaded as well, do not forget to call a function
 * from the io backend to set up the new streamer
 */
struct context *new_context(void)
{
    static struct ast *ast_root = NULL;
    struct context *c = malloc(sizeof(struct context));
    c->saved_file = swap_fd(NULL);
    c->saved_token = swap_next_token(NULL);
    c->saved_char = swap_next_char('\0');
    c->ast_root = AST_ROOT;
    ast_root = NULL;
    swap_ast_root(&ast_root);
    return c;
}

void load_context(struct context *c)
{
    // Swap AST
    struct ast *old_root = *AST_ROOT;
    swap_ast_root(c->ast_root);
    destroy_ast(old_root);

    swap_next_char(c->saved_char);
    free(swap_next_token(c->saved_token));
    fclose(swap_fd(c->saved_file));
    free(c);
}
