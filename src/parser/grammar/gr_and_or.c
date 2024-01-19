#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"
// and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
enum status gr_and_or(struct ast **ast)
{
    return gr_pipeline(ast);
}
