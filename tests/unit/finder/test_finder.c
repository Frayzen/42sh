#include <criterion/criterion.h>
#include <criterion/redirect.h>
void redirect_all_stdout(void)
{
  cr_redirect_stdout();
  cr_redirect_stderr();
}

Test(comments, at_end)
{
  char *test = "echo; 'coucou'\n#okokok";
    int off = 0;
    while (test[off] != 0)
    {
        char *string = finder(test,off);
        printf("%s\n", string);
        if (strlen(string) == 0)
          return 1;
        off += strlen(string);
        while (test[off] == ' ')
        {
            off++;
        }
    }
  cr_assert_stdout_eq_str("echo\n;\n'\ncoucou\n'\n\n");
}


Test(easy, just_words)
{
char *test = "hello world";
    int off = 0;
    while (test[off] != 0)
    {
        char *string = finder(test,off);
        printf("%s\n", string);
        if (strlen(string) == 0)
          return 1;
        off += strlen(string);
        while (test[off] == ' ')
        {
            off++;
        }
    }
  cr_assert_stdout_eq_str("hello world\n");
}

Test(medium, if)
{
char *test = "hello if";
    int off = 0;
    while (test[off] != 0)
    {
        char *string = finder(test,off);
        printf("%s\n", string);
        if (strlen(string) == 0)
          return 1;
        off += strlen(string);
        while (test[off] == ' ')
        {
            off++;
        }
    }
  cr_assert_stdout_eq_str("hello \nif\n");
}

Test(medium, if else then)
{
char *test = "hello world";
    int off = 0;
    while (test[off] != 0)
    {
        char *string = finder(test,off);
        printf("%s\n", string);
        if (strlen(string) == 0)
          return 1;
        off += strlen(string);
        while (test[off] == ' ')
        {
            off++;
        }
    }
  cr_assert_stdout_eq_str("if\nthen\nelse\n");
}
