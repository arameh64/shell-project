#include"includes.h"
int g_last_status = 0;

struct termios orig_termios;

void execute_commands(t_cmd *cmds, t_history **history)
{
    if (!cmds)
        return;

    if (cmds->next == NULL)
        execute_single(cmds, history);
    else
        execute_pipeline(cmds, history);
}

void execute_single(t_cmd *cmd, t_history **history)
{
    pid_t pid;

    if (is_builtin(cmd->argv[0]))
    {
        run_builtin(cmd, history);
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        handle_redirections(cmd);
        externals(cmd->argv);   // YOUR function
        exit(1);
    }
    
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    g_last_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    g_last_status = 128 + WTERMSIG(status);
}

void execute_pipeline(t_cmd *cmds, t_history **history)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    t_cmd *cmd = cmds;

    while (cmd)
    {
        pipe(fd);

        pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (cmd->next)
                dup2(fd[1], STDOUT_FILENO);

            close(fd[0]);
            close(fd[1]);

            handle_redirections(cmd);

            if (is_builtin(cmd->argv[0]))
                run_builtin(cmd, history);
            else
                externals(cmd->argv);

            exit(1);
        }

        if (prev_fd != -1)
            close(prev_fd);

        close(fd[1]);
        prev_fd = fd[0];

        cmd = cmd->next;
    }

    // wait for all children, last wait sets g_last_status
    while (waitpid(-1, &status, 0) > 0)
    {
        if (WIFEXITED(status))
            g_last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_last_status = 128 + WTERMSIG(status);
    }
}

void handle_redirections(t_cmd *cmd)
{
    int fd;

    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd < 0)
        {
            perror(cmd->infile);
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->outfile)
    {
        if (cmd->append)
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0)
        {
            perror(cmd->outfile);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void run_builtin(t_cmd *cmd, t_history **history)
{
    if (strcmp(cmd->argv[0], "cd") == 0)
        builtin_cd(cmd->argv);
    else if (strcmp(cmd->argv[0], "echo") == 0)
        builtin_echo(cmd->argv);
    else if (strcmp(cmd->argv[0], "pwd") == 0)
        builtin_pwd();
    else if (strcmp(cmd->argv[0], "exit") == 0)
        builtin_exit(history);
    else if (strcmp(cmd->argv[0], "env") == 0)
        builtin_env();
    else if (strcmp(cmd->argv[0], "export") == 0)
        builtin_export(cmd->argv);
    else if (strcmp(cmd->argv[0], "unset") == 0)
        builtin_unset(cmd->argv);
}

char *expand_var(char *str)
{
    int i = 0;

    while (str[i] && str[i] != '$')
        i++;

    if (!str[i])
        return strdup(str);

    // handle $?
    if (str[i + 1] == '?')
    {
        char buf[32];
        sprintf(buf, "%d", g_last_status);

        char result[1024];
        result[0] = 0;

        strncat(result, str, i);
        strcat(result, buf);
        strcat(result, str + i + 2);

        return strdup(result);
    }

    // handle $$
    if (str[i + 1] == '$')
    {
        char buf[32];
        sprintf(buf, "%d", getpid());

        char result[1024];
        result[0] = 0;

        strncat(result, str, i);
        strcat(result, buf);
        strcat(result, str + i + 2);

        return strdup(result);
    }

    // normal env var
    int start = i + 1;
    int len = 0;

    while (str[start + len] && (isalnum(str[start + len]) || str[start + len] == '_'))
        len++;

    char var[256];
    strncpy(var, str + start, len);
    var[len] = 0;

    char *val = getenv(var);
    if (!val)
        val = "";

    char result[1024];
    result[0] = 0;

    strncat(result, str, i);
    strcat(result, val);
    strcat(result, str + start + len);

    return strdup(result);
}

void expand_cmd_vars(t_cmd *cmd)
{
    int i = 0;

    while (cmd->argv[i])
    {
        if (strchr(cmd->argv[i], '$'))
        {
            char *new = expand_var(cmd->argv[i]);
            free(cmd->argv[i]);
            cmd->argv[i] = new;
        }
        i++;
    }
}


