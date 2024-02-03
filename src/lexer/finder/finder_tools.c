#include "finder_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "finder.h"
#include "io_backend/backend_saver.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

bool is_terminating(enum token_type type)
{
    switch (type)
    {
    case NEWLINE:
    case SEMI_COLON:
    case BSZERO:
        return true;
    default:
        return false;
    }
}

bool chevron_type(const struct lex_str *str)
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

bool is_name_char(char c)
{
    if (c == '_')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

bool is_name(char *str, size_t size)
{
    if (size == 0)
        return false;
    char c = str[0];
    if (!is_name_char(c))
        return false;
    if (c >= '0' && c <= '9')
        return false;
    for (size_t i = 1; i < size; i++)
    {
        c = str[i];
        // Check if c is in the portable character set
        if (is_name_char(c))
            continue;
        return false;
    }

    return true;
}

bool assignment_word(const struct lex_str *str)
{
    char *begin = str->value;
    char *first_eq = strchr(begin, '=');
    for (int i = 0; i < first_eq - str->value + 1; i++)
    {
        if (str->expand[i] != STR_LITTERAL)
            return false;
    }
    if (!first_eq || first_eq == str->value)
        return false;
    size_t size = first_eq - begin;
    return is_name(begin, size);
}

static enum expand_type get_expand_type(struct pending *p)
{
    if (p->in_sub_cmd)
        return p->in_quote ? QTD_SUB_CMD : SUB_CMD;
    else if (p->in_quote)
        return p->expanding ? QUOTED_VAR : QUOTED_STR;
    return p->expanding ? UNQUOTED_VAR : STR_LITTERAL;
}

void append_char(struct pending *p, char c)
{
    struct lex_str *str = &p->str;
    size_t id = str->size++;
    str->value = realloc(str->value, str->size * sizeof(char));
    str->value[id] = c;
    str->expand = realloc(str->expand, str->size * sizeof(enum expand_type));
    str->expand[id] = get_expand_type(p);
    p->blank = false;
}

// Also pop
void append_io(struct pending *p)
{
    append_char(p, io_peek());
    io_pop();
}

static const char limit_lt[] = {
    [SKIP_SINGLE_QUOTE] = '\'', [SKIP_DOUBLE_QUOTE] = '"',
    [SKIP_HASHTAG] = '\n',      [SKIP_VARIABLE_BRACKETS] = '}',
    [SKIP_PARENTHESES] = ')',
};

static void bslash_in_skip(struct pending *p, enum skip_behavior behavior)
{
    io_pop();
    if (io_peek() == limit_lt[behavior])
        append_io(p);
    else
    {
        if (behavior == SKIP_DOUBLE_QUOTE)
        {
            switch (io_peek())
            {
            case '\n':
                io_pop();
                return;
            case '$':
            case '`':
            case '\\':
                append_io(p);
                return;
            default:
                break;
            }
        }
        append_char(p, '\\');
        append_io(p);
    }
}

// Append every char until limit is found (limit excluded)
void skip_until(struct pending *p, enum skip_behavior behavior)
{
    char c = io_peek();
    bool append = behavior != SKIP_HASHTAG;
    while (c && c != limit_lt[behavior])
    {
        if (!append)
            io_pop();
        else if (behavior == SKIP_SINGLE_QUOTE)
            append_io(p);
        else if (behavior == SKIP_VARIABLE_BRACKETS && !is_name_char(c))
            exit_gracefully(BAD_VAR_NAME);
        else if (c == '\\')
            bslash_in_skip(p, behavior);
        else
        {
            if (limit_lt[behavior] == '"' && c == '$')
                consume_variable(p);
            else
                append_io(p);
        }
        c = io_peek();
    }
}

static void skip_sub_cmd_bs(struct pending *p)
{
    io_pop();
    if (io_peek() == ')')
        append_io(p);
    else
    {
        append_char(p, '\\');
        append_io(p);
    }
}

// append evething in the ()
void skip_sub_cmd(struct pending *p)
{
    char c = io_peek();
    int brac_cnt = 0;
    while (c && (c != ')' || brac_cnt != 0))
    {
        if (c == '\\')
        {
            skip_sub_cmd_bs(p);
            c = io_peek();
            continue;
        }
        else if (c == ')')
        {
            brac_cnt--;
            if (brac_cnt == 0)
            {
                append_io(p);
                return;
            }
        }
        else if (c == '(')
            brac_cnt++;
        append_io(p);
        c = io_peek();
    }
    if (brac_cnt != 0)
        exit_gracefully(UNEXPECTED_EOF);
}
