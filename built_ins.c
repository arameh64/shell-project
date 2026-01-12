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

int builtin_history(t_history *history)
{
    int i = 1;
    while (history)
    {
        printf("%d  %s\n", i++, history->cmd);
        history = history->next;
    }
    return 0;
}

int builtin_env()
{
      int i = 0;

    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }

    return 0;
}

int builtin_export(char **argv)
{
    int i = 1;

    if (!argv[1])
    {
        fprintf(stderr, "export: missing argument\n");
        return 1;
    }

    while (argv[i])
    {
        char *eq = strchr(argv[i], '=');

        if (!eq)
        {
            fprintf(stderr, "export: invalid format: %s\n", argv[i]);
            i++;
            continue;
        }

        *eq = '\0';                 // split
        char *key = argv[i];
        char *value = eq + 1;

        setenv(key, value, 1);

        *eq = '=';                  // restore (important)
        i++;
    }

    return 0;
}

int builtin_unset(char **argv)
{
    int i = 1;

    if (!argv[1])
    {
        fprintf(stderr, "unset: missing argument\n");
        return 1;
    }

    while (argv[i])
    {
        unsetenv(argv[i]);
        i++;
    }

    return 0;
}
