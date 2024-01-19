#include "rules.h"
#include "tools/ast/ast.h"
enum status gr_and_or(struct ast_list *list)
{
    return gr_pipeline(list);
}
