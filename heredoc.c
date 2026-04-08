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

    setup_signals_heredoc();
    if (pipe(pipe_fd) < 0)
        return (-1);
    read_heredoc_input(rd, shell, pipe_fd[1]);
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}
int apply_heredoc(t_redi *rd, t_shell *shell)
{
    int fd = handle_heredoc(rd, shell);
    if (fd < 0)
        return (-1);
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}