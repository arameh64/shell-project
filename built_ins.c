#include "includes.h"


int builtin_pwd(void)
{
    char buf;
    
    if(getcwd(buf , sizeof(buf)) == NULL)
    {
        perror("pwd");
        return 1;
    }
    
    printf("%s" , buf);
return 0;    
}

int builtin_cd(char **argv)
{
    char *target;
    char oldpwd[4096];
    char newpwd[4096];

    if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
    {
        perror("cd");
        return 1;
    }

    if (argv[1] == NULL || strcmp(argv[1], "~") == 0)
    {
        target = getenv("HOME");
    }
    else if (strcmp(argv[1], "-") == 0)
    {
        target = getenv("OLDPWD");
    }
    else
    {
        target = argv[1];
    }

    if (!target)
    {
        fprintf(2 , "cd: target not set\n");
    }
}

int builtin_clear(void)
{
    printf("\033[H\033[J");
    return 0;
}

int builtin_exit( char ** argv )

{
    int status = 0;
    if(argv[1])
    {
        status = argv[1];
    }
    
    exit(status);
}

int builtin_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    if (argv[1] && strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }

    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");

    return 0;
}

void add_history(t_history **head, char *line)
{
    t_history *new = malloc(sizeof(t_history));
    new->cmd = strdup(line);
    new->next = NULL;

    if (!*head)
    {
        *head = new;
        return;
    }

    t_history *tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}




