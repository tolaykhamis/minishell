#include "minishell.h"


static void child_process(t_shell *shell, t_cmdlist *cmd,
                            int prev_fd, int *pipe_fd)
{
    char *path;
    int status;

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
    if (!cmd->av || !cmd->av[0])
		exit(0);
    init_isbuiltin(cmd);
    if (cmd->is_builtin)
    {
        if (ft_strcmp(cmd->av[0], "exit") == 0)
        {
            if (cmd->av[1])
                status = ft_atoi(cmd->av[1]);
            else
                status = shell->exit_status;
            exit(status);
        }
        else
        exit(execute_builtin(shell, cmd));
}

    if (ft_strchr(cmd->av[0], '/'))
        path = cmd->av[0];
    else
        path = command_path(cmd->av[0], shell->envp,0, shell);
    if (!path)
    {
        put_str_fd("minishell: ", 2);
        put_str_fd(cmd->av[0], 2);
        put_str_fd(": command not found\n", 2);
        free(shell->pids);
        clean_before_exit(shell);
        exit(127);
    }
    execve(path, cmd->av, shell->envp);
    if (path != cmd->av[0])
		free(path);
    perror(cmd->av[0]);
    exit(126);
}
static void execute_pipeline_cmd(t_shell *shell, t_cmdlist *cmd,
                                 int prev_fd, int *pipe_fd, pid_t *pid)
{
    *pid = fork();
    if (*pid == 0)
    {
        setup_signals_child();
        child_process(shell, cmd, prev_fd, pipe_fd ? pipe_fd : NULL);
    }

    if (prev_fd != STDIN_FILENO)
        close(prev_fd);

    if (pipe_fd)
    {
        close(pipe_fd[1]);
    }
}

void pipeline_execution(t_shell *shell,int i)
{
    t_cmdlist *cmd;
    int pipe_fd[2];
    int prev_fd;
    // pid_t *pids;
    int count;

    count = cmds_len(shell->cmds);
    shell->pids = init_pids(count);
    if (!shell->pids)
        return;
    prev_fd = STDIN_FILENO;
    cmd = shell->cmds;
    while (cmd)
    {
        if (cmd->next)
            pipe(pipe_fd);
        execute_pipeline_cmd(shell, cmd, prev_fd, cmd->next ? pipe_fd : NULL, &shell->pids[i]);
        wait_all(shell, shell->pids, count);
        if (cmd->next)
            prev_fd = pipe_fd[0];
        cmd = cmd->next;
        i++;
    }
	// printf("Hereeeeeeee");
    // clean_before_exit(shell);
    // wait_all(shell, shell->pids, count);
    free(shell->pids);
}
