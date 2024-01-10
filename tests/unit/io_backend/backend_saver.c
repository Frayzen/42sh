#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include "io_backend/backend_saver.h"

TestSuite(backend_saver);

Test(backend_saver, basic_put)
{
    io_put("test");
    cr_assert_eq(io_get_char(), 't');
}
