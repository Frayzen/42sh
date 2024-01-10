#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "finder.h"
#define CHECK_SPECIAL_CHAR(Char)(((Char) == '\n' || (Char) == ';' || (Char) == '\'')? 1:0)




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
* check_reserved: checks if the word given is one of the reserved word
* @param pending: a char*, the word we are inspecting
* @return a boolean, 1 if it is a special word, 0 otherwise
*/
static int check_reserved(char *pending)
{
   char c = get_backend();
   for (int i = 0; i < MAX_TYPE; i++)
    {
        if (!strcmp(pending, type_names[i]))
        {
            if (c != ' ')
                return 0;
            return 1;
        }
    }
    return 0;
}

char *finder()
{
    char *pending = malloc(2); //one character + terminating NULL to check with strcmp

    size_t size_pending = 0;
    char c = get_backend();
    offset += 1;
    if (c == ' ')
        c = get_backend();
    pending[0] = c;
    size_pending++;
    pending[size_pending] = 0;

    while (!check_reserved(pending))
    {
        //TODO pop_backend();
        c = get_backend();
        offset += 1;
        if (c == ' ' || !(CHECK_SPECIAL_CHAR(c)))
        {
            pending = realloc(pending, size_pending + 1);
            pending[size_pending] = c;
            size_pending++;
            pending[size_pending] = 0;
            if (c == ' ')
            {
                //TODO pop_backend();
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

