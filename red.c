# include "minishell.h"

static int  handle_heredoc(t_redi *rd, t_shell *shell)
{
    int     pipe_fd[2];
    char    *line;
    char    *expanded;

    setup_signals_heredoc();
    if (pipe(pipe_fd) < 0)
        return (-1);
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
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);  
}

int apply_redirections(t_cmdlist *cmd, t_shell *shell)
{
    t_redi  *rd;
    int     fd;

    rd = cmd->redirs;
    while (rd)
    {
        if (rd->type == TOKEN_REDIRECT_IN)
        {
            fd = open(rd->f, O_RDONLY);
            if (fd < 0)
            {
                put_str_fd("minishell: ", 2);
                put_str_fd(rd->f, 2);
                put_str_fd(": No such file or directory\n", 2);
                return (-1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (rd->type == TOKEN_REDIRECT_OUT)
        {
            fd = open(rd->f, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                put_str_fd("minishell: ", 2);
                put_str_fd(rd->f, 2);
                put_str_fd(": Permission denied\n", 2);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (rd->type == TOKEN_APPEND)
        {
            fd = open(rd->f, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                put_str_fd("minishell: ", 2);
                put_str_fd(rd->f, 2);
                put_str_fd(": Permission denied\n", 2);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (rd->type == TOKEN_HERDOC)
        {
            fd = handle_heredoc(rd, shell);
            if (fd < 0)
                return (-1);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        rd = rd->next;
    }
    return (0);
}