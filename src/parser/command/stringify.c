#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env/context.h"
#include "env/env.h"
#include "env/vars/specials.h"
#include "env/vars/vars.h"
#include "exec/execs.h"
#include "exit/error_handler.h"
#include "expander.h"
#include "grammar/rules.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "sys/wait.h"
#include "unistd.h"

// Find the first delimiter
static int is_in(char c, char *delims)
{
    if (!c)
        return false;
    int i = 0;
    while (delims[i] && delims[i] != c)
        i++;
    return delims[i] == c;
}

// Stops at each delimiter
static char *my_str_tok(char *buf, char *delims)
{
    static char *str = NULL;
    if (buf)
    {
        char *s = buf;
        while (*buf)
            buf++;
        while (buf > s && is_in(*(buf - 1), delims)
               && is_in(*(buf - 1), DEFAULT_IFS))
            buf--;
        *buf = '\0';
        str = s;
    }
    if (!str)
        return NULL;
    // Skip all leading spaces in delim
    while (is_in(*str, delims) && is_in(*str, DEFAULT_IFS))
        str++;
    char *b = str;
    // Get next delim
    int i = 0;
    while (str[i] && !is_in(str[i], delims))
        i++;
    char *e = str + i;
    // Skip all trailing spaces and go to next
    while (is_in(str[i], delims) && is_in(str[i], DEFAULT_IFS))
        i++;
    // If not trailing space
    if (is_in(str[i], delims))
        i++;

    if (!str[i])
        str = NULL;
    else
    {
        *e = '\0';
        str += i;
    }
    return b;
}

static struct expandable *ifs_splitting(char *str, struct expandable *cur)
{
    char *ifs = DEFAULT_IFS;
    if (is_set_var("IFS"))
        ifs = read_var("IFS");
    char *elem = my_str_tok(str, ifs);
    struct expandable *last = NULL;
    struct expandable *first = NULL;
    if (!elem)
    {
        last = expandable_init(str, STR_LITTERAL, cur->link_next);
        last->next = cur->next;
        return last;
    }
    while (elem)
    {
        struct expandable *new_string =
            expandable_init(strdup(elem), STR_LITTERAL, false);
        if (last)
            last->next = new_string;
        else
            first = new_string;
        last = new_string;
        elem = my_str_tok(NULL, ifs);
    }
    last->link_next = cur->link_next;
    last->next = cur->next;
    free(str);
    return first;
}

// Expand the unquoted var (= expand it and split it by ' ')
// return the new current or NULL if the expansion is empty
static struct expandable *expand_unquoted_var(struct expandable *cur)
{
    char *val = retrieve_var(cur->content);
    if (val[0] == '\0')
    {
        free(val);
        return NULL;
    }
    return ifs_splitting(val, cur);
}

// Expand the quoted var (= expand it)
// return the new current or NULL if the expansion is empty
static struct expandable *expand_quoted_var(struct expandable *cur)
{
    assert(cur->type == QUOTED_VAR);
    char *content = retrieve_var(cur->content);
    if (content[0] == '\0')
    {
        free(content);
        return NULL;
    }
    struct expandable *new_string =
        expandable_init(content, STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

// Expand the string litteral (= duplicate it)
static struct expandable *expand_str_litt(struct expandable *cur)
{
    assert(IS_STR_TYPE(cur->type));
    struct expandable *new_string =
        expandable_init(strdup(cur->content), STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}
static void process_buffer(char *buf)
{
    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == ' '))
        len--;

    for (size_t i = 0; i < len; i++)
        if (buf[i] == '\n' || buf[i] == ' ')
            buf[i] = ' ';
    buf[len] = '\0';
}

#define BUFSIZE 8
static struct expandable *expand_sub_cmd(struct expandable *cur)
{
    int fds[2];
    int err = pipe(fds); // create the pipe reading stdout for the subcmd
                         // (child) into a buffer
    if (err == -1)
        exit(3);
    int pid = fork();
    if (!pid)
    {
        close(fds[0]);
        if (dup2(fds[1], STDOUT_FILENO) == -1)
        {
            perror("dup2 failed in child");
            exit(EXIT_FAILURE);
        }
        close(fds[1]);
        DBG_PIPE("set STDOUT to %d\n", STDOUT);
        struct context *old = new_context();
        io_streamer_string(cur->content); // set the input cmd for the subcmd
        DBG_PIPE("shit\n");
        if (gr_input(AST_ROOT) == ERROR)
        {
            *AST_ROOT = NULL;
            print_error(GRAMMAR_ERROR_ENTRY);
        }
        else
            exec_entry(*AST_ROOT);
        close(fds[0]);
        load_context(old);
        _exit(0);
    }
    char *buf = calloc(BUFSIZE, sizeof(char));
    close(fds[1]);
    int begin = 0;
    ssize_t size;
    while ((size = read(fds[0], buf + begin, BUFSIZE)) > 0)
    {
        begin += size;
        buf = realloc(buf, begin + BUFSIZE);
    }
    buf[begin] = '\0';
    process_buffer(buf); // newlines to spaces accroding to SCL

    int ret;
    waitpid(pid, &ret, 0);

    if (!strcmp("", buf))
    {
        free(buf);
        return NULL;
    }

    cur = ifs_splitting(
        buf, cur); // if buf has spaces, split into separate expandables
    return cur;
}

// This function calls the appropriate subfunction in order to create a string
// litteral linked list from the currrent expandable
struct expandable *stringify_expandable(struct expandable *cur)
{
    int valid = 0;
    struct expandable *exp = expand_special_var(cur, &valid);
    if (valid)
        return exp;
    switch (cur->type)
    {
    case QUOTED_VAR:
        return expand_quoted_var(cur);
    case UNQUOTED_VAR:
        return expand_unquoted_var(cur);
    case SUB_CMD:
        return expand_sub_cmd(cur);
    default:
        return expand_str_litt(cur);
    }
}
