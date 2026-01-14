#include "includes.h"

void free_cmds(t_cmd *cmds)
{
    int i;

    while (cmds)
    {
        t_cmd *next = cmds->next;

        /* free argv */
        if (cmds->argv)
        {
            i = 0;
            while (cmds->argv[i])
            {
                free(cmds->argv[i]);
                i++;
            }
            free(cmds->argv);
        }

        /* free redirections */
        if (cmds->infile)
            free(cmds->infile);

        if (cmds->outfile)
            free(cmds->outfile);

        /* free the struct */
        free(cmds);

        cmds = next;
    }
}

void print_tokens(t_token *t)
{
    while (t)
    {
        if (t->type == TOK_WORD)
            printf("WORD(%s)\n", t->value);
        else if (t->type == TOK_PIPE)
            printf("PIPE\n");
        else if (t->type == TOK_REDIR_IN)
            printf("REDIR_IN\n");
        else if (t->type == TOK_REDIR_OUT)
            printf("REDIR_OUT\n");
        else if (t->type == TOK_REDIR_APPEND)
            printf("REDIR_APPEND\n");
        else if (t->type == TOK_BACKGROUND)
            printf("BACKGROUND\n");

        t = t->next;
    }
}

void free_tokens(t_token *t)
{
    while (t)
    {
        t_token *n = t->next;
        free(t->value);
        free(t);
        t = n;
    }
}


