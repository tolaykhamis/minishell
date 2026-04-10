#include "minishell.h"

static void read_heredoc_input(t_redi *rd, t_shell *shell, int write_fd)
{
    char    *line;
    char    *expanded;


    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (ft_strcmp(line, rd->f) == 0)
        {
            free(line);
            break ;
        }
        if (!rd->has_quotes)
        {
            expanded = expand_parameter(line, shell);
            free(line);
            line = expanded;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
}
static int  handle_heredoc(t_redi *rd, t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid;
    int status;

    if (pipe(pipe_fd) < 0)
        return (-1);
    pid = fork();
    if (pid == 0)
    {
        setup_signals_heredoc();
        close(pipe_fd[0]);
        read_heredoc_input(rd, shell, pipe_fd[1]);
        close(pipe_fd[1]);
        exit(0);
    }
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        shell->exit_status = 130; 
        close(pipe_fd[0]);
        return (-1);
    }
    return (pipe_fd[0]);
}
int apply_heredoc(t_redi *rd, t_shell *shell)
{
   (void)shell;

    if (dup2(rd->heredoc_fd, STDIN_FILENO) < 0)
        return (-1);
    // else
    // {
    //     if (handle_heredoc(rd, shell) < 0)
    //         return -1;
    // }
    close(rd->heredoc_fd);
    return (0);
}
int preprocess_heredocs(t_cmdlist *cmds, t_shell *shell)
{
    t_cmdlist *cmd;
    t_redi *rd;

    cmd = cmds;
    while (cmd)
    {
        rd = cmd->redirs;
        while (rd)
        {
            if (rd->type == TOKEN_HERDOC)
            {
                rd->heredoc_fd = handle_heredoc(rd, shell);
                if (rd->heredoc_fd < 0)
                    return (-1);
            }
            rd = rd->next;
        }
        cmd = cmd->next;
    }
    return (0);
}
