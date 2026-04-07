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
static int redirect_fd(const char *file, int flags, mode_t mode, int target_fd)
{
    int fd = open(file, flags, mode);  
    if (fd < 0)
    {
        put_str_fd("minishell: ", 2);
        put_str_fd((char *)file, 2);
        put_str_fd(": ", 2);
        if (flags & O_RDONLY)
            put_str_fd("No such file or directory\n", 2);
        else
            put_str_fd("Permission denied\n", 2);
        return (-1);
    }
    dup2(fd, target_fd); 
    close(fd);
    return 0;
}

static int apply_heredoc(t_redi *rd, t_shell *shell)
{
    int fd = handle_heredoc(rd, shell);
    if (fd < 0)
        return -1;
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}
int apply_redirections(t_cmdlist *cmd, t_shell *shell)
{
    t_redi *rd = cmd->redirs;

    while (rd)
    {
        if (rd->type == TOKEN_REDIRECT_IN)
        {
            if (redirect_fd(rd->f, O_RDONLY, 0, STDIN_FILENO) < 0)
                return -1;
        }
        else if (rd->type == TOKEN_REDIRECT_OUT)
        {
            if (redirect_fd(rd->f, O_WRONLY | O_CREAT | O_TRUNC, 0644, STDOUT_FILENO) < 0)
                return -1;
        }
        else if (rd->type == TOKEN_APPEND)
        {
            if (redirect_fd(rd->f, O_WRONLY | O_CREAT | O_APPEND, 0644, STDOUT_FILENO) < 0)
                return -1;
        }
        else if (rd->type == TOKEN_HERDOC)
        {
            if (apply_heredoc(rd, shell) < 0)
                return -1;
        }
        rd = rd->next;
    }
    return 0;
}