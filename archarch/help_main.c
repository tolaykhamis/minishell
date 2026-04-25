#include "minishell.h"

void	clean_before_exit(t_shell *shell)
{
	close_fds(3);
	free_free(shell->envp);
	free_free(shell->export);
	freeargs(shell->cmds);
	clear_pids(shell);
	shell->cmds = NULL;
	shell->envp = NULL;
	shell->export = NULL;
	rl_clear_history();
}

void clear_pids(t_shell *shell)
{
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
}

void	close_fds(int fd)
{
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}
