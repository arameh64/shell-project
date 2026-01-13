#include"includes.h"

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
    waitpid(pid, NULL, 0);
}

void execute_pipeline(t_cmd *cmds, t_history **history)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    t_cmd *cmd = cmds;

    while (cmd)
    {
        pipe(fd);

        pid = fork();
        if (pid == 0)
        {
            // if not first command, read from previous pipe
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // if not last command, write to next pipe
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

    while (wait(NULL) > 0);
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




