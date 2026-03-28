#include "minishell.h"

void execute_external(t_shell *shell,t_cmdlist *cmd)
{
    pid_t pid;
    char *path;
    int status;

    if (ft_strchr(cmd->av[0], '/'))
        path = cmd->av[0];
    else
        path = command_path(cmd->av[0], shell->envp);

    if (!path)
    {
        put_str_fd("minishell: ", 2);
        put_str_fd(cmd->av[0], 2);
        put_str_fd(": command not found\n", 2);
        shell->exit_status = 127;
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        execve(path, cmd->av, shell->envp);
        perror(cmd->av[0]);
        exit(126);
    }
    else if(pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
    }
    else 
    {
        perror("fork");
        shell->exit_status = 1;
    }
    if (path != cmd->av[0])
        free(path);
}