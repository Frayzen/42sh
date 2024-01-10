#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <finder.h>

static const char *const type_names[] =
{
      [IF] = "if",
      [THEN] = "then",
      [ELIF] = "elif",
      [ELSE] = "else",
      [FI] = "fi",
      [SEMI_COLON] = ";",
      [BACK_N] = "\n",
      [SINGLE_QUOTE] = "'",
};

/***
* check_terminal: checks if the word given is one of the reserved word
* @param pending: a char*, the word we are inspecting
* @return a boolean, 1 if it is a special word, 0 otherwise
*/
static int check_terminal(char *pending)
{
    char c = get_backend();
   for (int i = 0; i < 8; i++)
    {
        if (strcmp(pending, type_names[i]) == 0)
        {
            if (c != ' ')
                return 0;
            return 1;
        }
    }
    return 0;
}


/***
* check_special_one_chara: check if the character given is one of the reserved
* one
* @param chara: the character we are looking at
* @return a boolean, 1 if it is reserved, 0 otherwise
*/

static int check_special_one_chara(char chara)
{
    return (chara == '\n' || chara == ';' || chara == '\'') ? 1: 0;
}


/***
* finder: gets character by character until the word is recognizable, or there
* @get_backend function: to look at the next character without popping it
* @pop_backend function: pops the character in backend so the next one is
* available
* @return the string to be used for the token
*/

char *finder(char *string, int offset) //les arguments sont juste pour tester, ce sera void normalement
{
    char *pending = malloc(2); //one character + terminating NULL to check with strcmp

    size_t size_pending = 0;
    char c = get_backend(string, offset);
    offset += 1;
    if (c == ' ')
        c = get_backend(string, offset);
    pending[0] = c;
    size_pending++;
    pending[size_pending] = 0;
    while (!check_terminal(pending, string, offset))
    {
        //pop_backend();
        c = get_backend(string, offset);
        offset += 1;
        if (c == ' ' || !check_special_one_chara(c))
        {
            pending = realloc(pending, size_pending + 1);
            pending[size_pending] = c;
            size_pending++;
            pending[size_pending] = 0;
            if (c == ' ')
            {
                //pop_backend();
                return pending;
            }
        }
        else
        {
            return pending;
        }
    }
    return pending;
}

/*
int main(void)
{
    char *test = "ifp; 'coucou'\n";
    int off = 0;
    while (test[off] != 0)
    {
        char *string = finder(test,off);
        printf("%s\n", string);
        off += strlen(string);
        while (test[off] == ' ')
        {
            off++;
        }
    }
}
*/
/*
 right now, the single quotes give out singular tokens,
 but it can be modified to get one big token
 containing all that is between the quotes
 */
