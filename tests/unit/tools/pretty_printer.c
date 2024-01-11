#include "tools/pretty_printer.h"

#include <criterion/criterion.h>
#include <criterion/internal/redirect.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
}

Test(pretty_printer, basic, .init = redirect_all_stdout)
{
    struct token *ifToken = init_token("if");
    struct token *conditionToken = init_token("true");
    struct token *thenToken = init_token("then");
    struct token *bodyToken = init_token("echo");
    struct token *semicolonToken = init_token(";");
    struct ast *ifNode = init_ast(ifToken);
    struct ast *conditionNode = init_ast(conditionToken);
    struct ast *thenNode = init_ast(thenToken);
    struct ast *bodyNode = init_ast(bodyToken);
    struct ast *semicolonNode = init_ast(semicolonToken);
    ifNode->nb_children = 2;
    ifNode->children = malloc(ifNode->nb_children * sizeof(struct ast *));
    ifNode->children[0] = conditionNode;
    ifNode->children[1] = thenNode;
    thenNode->nb_children = 2;
    thenNode->children = malloc(thenNode->nb_children * sizeof(struct ast *));
    thenNode->children[0] = bodyNode;
    thenNode->children[1] = semicolonNode;
    pretty_print_ast(ifNode);
    destroy_ast(ifNode);
    cr_assert_stdout_eq_str("\nif\n╠══true\n╚══then\n   ╠══echo\n   ╚══;\n");
}
