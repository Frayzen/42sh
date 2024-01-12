#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "io_backend/backend_saver.h"
#include "lexer/finder.h"

void redirect_all_stdout(void)
{
        cr_redirect_stdout();
        cr_redirect_stderr();
}

Test(echo, one word)
{
    main_to_stream(3, "./42sh echo toto");
    struct ast *e = NULL;
    gr_input(&e);
    echo_function(e);
    cr_assert_stdout_eq_str("toto\n");
}
