#include "minishell.h"

void execute_command(t_shell *shell)
{
     t_cmdlist *cmd;

    cmd = shell->cmds;
    if (cmd == NULL)
        return ;
    if (cmd->next == NULL)
        single_execution(shell, cmd);
    else
        pipeline_execution(shell);
}
void single_execution(t_shell *shell, t_cmdlist *cmd)
{
    int     saved_stdin;
    int     saved_stdout;

    saved_stdin  = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    init_isbuiltin(cmd);
    if (cmd->redirs)
    {
        if (apply_redirections(cmd, shell) < 0)
        {
            shell->exit_status = 1;
            dup2(saved_stdin,  STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            return ;
        }
    }
    if (cmd->is_builtin)
        shell->exit_status = execute_builtin(shell, cmd);
    else
        execute_external(shell, cmd);

    dup2(saved_stdin,  STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

