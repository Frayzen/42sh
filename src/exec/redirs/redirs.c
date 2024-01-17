#include "redirs.h"
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

int create_fd(char *str, bool is_io, int flags)
{
    int ret = -1;
    if (is_io)
        ret = atoi(str);
    else
        ret = open(str, flags);
    return ret;
}

void apply_redirection(struct sh_command *cmd, struct ast *redir)
{
    assert(redir->type == AST_REDIR && redir->nb_children >= 2);
    int i = 0;
    //PARSE FROM
    char* from = NULL;
    struct token *token = redir->children[i++]->token;
    if(token->type == CHEVRON)
    {
        if (token->value[0] == '<')
            from = "1";
        else if (token->value[0] == '>')
            from = "0";
    }else
        from = token->value;
    token = redir->children[i++]->token;
    //PARSE CHEVRON
    bool is_io = token->value[1] == '&';
    bool append = token->value[1] == '>';
    bool both_way = token->value[1] == '<';
    is_io &= token->type == IO_NUMBER;
    token = redir->children[i++]->token;
    //PARSE TO
    char *to = token->value;
    is_io = (token->type != WORD) && is_io;
    int from_fd = create_fd(from, true, append);
    int to_fd = create_fd(to, is_io, false);
    assert(from_fd != -1 && to_fd != -1);
}
