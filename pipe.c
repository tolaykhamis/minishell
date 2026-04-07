#include "minishell.h"

static int  cmds_len(t_cmdlist *cmds)
{
    int         i;
    t_cmdlist   *tmp;

    i = 0;
    tmp = cmds;
    while (tmp)
    {
        tmp = tmp->next;
        i++;
    }
    return (i);
}

static pid_t    *init_pids(int count)
{
    pid_t   *pids;

    pids = ft_calloc(count, sizeof(pid_t));
    return (pids);
}

static void child_process(t_shell *shell, t_cmdlist *cmd,
                            int prev_fd, int *pipe_fd)
{
    char *path;

    if (prev_fd != STDIN_FILENO)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd->next && pipe_fd)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    if (cmd->redirs)
    {
        if (apply_redirections(cmd, shell) < 0)
            exit(1);
    }
    init_isbuiltin(cmd);
    if (cmd->is_builtin)
        exit(execute_builtin(shell, cmd));
    if (ft_strchr(cmd->av[0], '/'))
        path = cmd->av[0];
    else
        path = command_path(cmd->av[0], shell->envp,0);
    if (!path)
    {
        put_str_fd("minishell: ", 2);
        put_str_fd(cmd->av[0], 2);
        put_str_fd(": command not found\n", 2);
        exit(127);
    }
    execve(path, cmd->av, shell->envp);
    perror(cmd->av[0]);
    exit(126);
}

static void wait_all(t_shell *shell, pid_t *pids, int count)
{
    int i;
    int status;

    i = 0;
    while (i < count)
    {
        waitpid(pids[i], &status, 0);
        if (i == count - 1)
        {
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
        }
        i++;
    }
}

void    pipeline_execution(t_shell *shell)
{
    t_cmdlist   *cmd;
    int         pipe_fd[2];
    int         prev_fd;
    pid_t       *pids;
    int         count;
    int         i;

    count = cmds_len(shell->cmds);
    pids = init_pids(count);
    if (!pids)
        return ;
    prev_fd = STDIN_FILENO;
    cmd = shell->cmds;
    i = 0;
    while (cmd)
    {
        if (cmd->next)
            pipe(pipe_fd);
        pids[i] = fork();
        if (pids[i] == 0)
        {
            setup_signals_child();
            child_process(shell, cmd, prev_fd, cmd->next ? pipe_fd : NULL);
        }
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        cmd = cmd->next;
        i++;
    }
    wait_all(shell, pids, count);
    free(pids);
}
