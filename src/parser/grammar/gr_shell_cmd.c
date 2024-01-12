#include <stddef.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_shell_cmd(struct ast **ast)
{
    return gr_if(ast);
}

