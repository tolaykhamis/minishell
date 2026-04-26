#include "minishell.h"

void	clean_before_exit(t_shell *shell)
{
	close_fds(3);
	freeargs(shell->cmds);
	free_free(shell->export);
	if (shell->envp)
		free_free(shell->envp);
	rl_clear_history();
	shell->cmds = NULL;
	shell->envp = NULL;
	shell->export = NULL;
	clear_pids(shell);
}

void clear_pids(t_shell *shell)
{
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
}
void	clean_child_exit(t_shell *shell)
{
    close_fds(3);
    freeargs(shell->cmds);
    rl_clear_history();
    shell->cmds = NULL;
    clear_pids(shell);

}
void	close_fds(int fd)
{
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}
