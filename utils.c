#include "includes.h"

void execute_commands(t_cmd *cmds, t_history **history)
{
while (cmds)
{
        char **argv = cmds->argv;
    
        if (!argv || !argv[0])
        {
            cmds = cmds->next;
            continue;
        }
    
        if (strcmp(argv[0], "cd") == 0)
            builtin_cd(argv);
        else if (strcmp(argv[0], "pwd") == 0)
            builtin_pwd();
        else if (strcmp(argv[0], "exit") == 0)
            builtin_exit(argv);
        else if (strcmp(argv[0], "echo") == 0)
            builtin_echo(argv);
        else if (strcmp(argv[0], "env") == 0)
            builtin_env();
        else if (strcmp(argv[0], "export") == 0)
            builtin_export(argv);
        else if (strcmp(argv[0], "unset") == 0)
            builtin_unset(argv);
        else if (strcmp(argv[0], "history") == 0)
            builtin_history(*history);
        else
        {
            // external command
            pid_t pid = fork();
    
            if (pid == 0)
            {
                execvp(argv[0], argv);
                perror("exec");
                exit(1);
            }
            else
                waitpid(pid, NULL, 0);
        }
    
        cmds = cmds->next;
    }
}

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
