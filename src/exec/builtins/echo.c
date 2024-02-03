#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include "env/env.h"

void print_echo(char **argv, int i, int interpret_bslash, int print_nline)
{
    DBG_PIPE("Echo command [OUT] %d\n", STDOUT);
    for (; argv[i]; i++)
    {
        const char *content = argv[i];
        int id = 0;
        while (content[id])
        {
            if (content[id] == '\\' && interpret_bslash)
            {
                id++;
                switch (content[id])
                {
                case 'n':
                    dprintf(STDOUT, "\n");
                    break;
                case '\\':
                    dprintf(STDOUT, "\\");
                    break;
                case 't':
                    dprintf(STDOUT, "\t");
                    break;
                default:
                    continue;
                }
            }
            else
                dprintf(STDOUT, "%c", content[id]);
            id++;
        }
        if (argv[i + 1])
            dprintf(STDOUT, " ");
    }
    if (print_nline)
        dprintf(STDOUT, "\n");
}

/***
 * sets the options according to the content sequence
 * returns TRUE_B if the sequence sets options and FALSE_B otherwise
 * ie: if content does not start with '-' or contains a char that is not 'n' 'e'
 * or 'E'
 */
int set_option_echo(const char *content, int *interpret_bslash,
                    int *print_nline)
{
    int init_bslash = *interpret_bslash;
    int init_print = *print_nline;
    if (content[0] != '-')
        return FALSE_B;
    for (int i = 1; content[i] != '\0'; i++)
    {
        switch (content[i])
        {
        case 'n':
            *print_nline = FALSE_B;
            break;
        case 'e':
            *interpret_bslash = TRUE_B;
            break;
        case 'E':
            *interpret_bslash = FALSE_B;
            break;
        default:
            *print_nline = init_print;
            *interpret_bslash = init_bslash;
            return FALSE_B;
        }
    }
    return TRUE_B;
}

void builtin_echo(char **argv)
{
    int print_nline = TRUE_B;
    int interpret_bslash = FALSE_B;
    int i = 1;
    while (argv[i])
    {
        DBG_VAR("[ECHO] ARG[%d] = %s\n", i, argv[i]);
        const char *content = argv[i];
        if (!set_option_echo(content, &interpret_bslash, &print_nline))
            break;
        i++;
    }
    print_echo(argv, i, interpret_bslash, print_nline);
    fflush(NULL);
}
