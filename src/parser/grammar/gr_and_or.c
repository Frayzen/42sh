#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
// and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;

enum status gr_and_or(struct ast **ast)
{
    GR_DBG_START(AndOr);
    GR_DBG_RET(gr_pipeline(ast));
}
