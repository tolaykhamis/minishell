#include "minishell.h"

int  cmds_len(t_cmdlist *cmds)
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

pid_t    *init_pids(int count)
{
    pid_t   *pids;

    pids = ft_calloc(count, sizeof(pid_t));
    return (pids);
}
void wait_all(t_shell *shell, pid_t *pids, int count)
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