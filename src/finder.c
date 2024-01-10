#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

enum type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMI_COLON,
    BACK_N,
    SINGLE_QUOTE,
    WORD,
    MAX_TYPE,
};

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


static char backend(char *string, int offset)
{
    return string[offset];
}


//check_terminal: at each iteration of the main loop of finder, it checks if the
//string is a recognizable word

static int check_terminal(char *pending, char *string, int offset)
{
    //printf("inside check: %s\n", pending);
    char c = backend(string, offset);
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



//check_special_one_chara: checks if the character is a special word

static int check_special_one_chara(char c)
{
    return (c == '\n' || c == ';' || c == 39) ? 1: 0; //39 is the ASCII code for a single quote
}



//finder: gets character by character until the word is recognizable, or there
//is a space

char *finder(char *string, int offset) //les arguments sont juste pour tester, ce sera void normalement
{
    char *pending = malloc(2); //one character + terminating NULL to check with strcmp

    size_t size_pending = 0;
    char c = backend(string, offset);
    offset += 1;
    if (c == ' ')
        c = backend(string, offset);
    pending[0] = c;
    size_pending++;
    pending[size_pending] = 0;
    while (!check_terminal(pending, string, offset))
    {
        //pop_backend();
        c = backend(string, offset);
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

/*
 right now, the single quotes give out singular tokens,
 but it can be modified to get one big token
 containing all that is between the quotes
 */
