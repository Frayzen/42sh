#include "lexer_tools.h"
#include <string.h>
#include "tools/str/string.h"
#include "tools/token/token.h"

bool is_terminating(struct token *token)
{
    switch (token->type)
    {
    case NEWLINE:
    case SEMI_COLON:
    case BSZERO:
        return true;
    default:
        return false;
    }
}

bool chevron_type(const struct string *str)
{
    if (!strcmp(">", str->value))
        return 1;
    if (!strcmp("<", str->value))
        return 1;
    if (!strcmp(">>", str->value))
        return 1;
    if (!strcmp(">&", str->value))
        return 1;
    if (!strcmp("<&", str->value))
        return 1;
    if (!strcmp(">|", str->value))
        return 1;
    if (!strcmp("<>", str->value))
        return 1;
    return 0;
}

bool is_name(char *str, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        char c = str[i];
        // Check if c is in the portable character set
        if (c == '\0')
            continue;
        if (c >= '\a' && c <= '\r')
            continue;
        if (c >= ' ' && c <= '~')
            continue;
        return false;
    }
    return true;
}

bool assigment_word(const struct string *str)
{
    char *begin = str->value;
    char* first_eq = strchr(begin, '=');
    if (!first_eq)
        return false;
    size_t size = first_eq - begin + 1;
    return is_name(begin, size);
}
