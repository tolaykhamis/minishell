
#include "minishell.h"

void init_isbuiltin(t_cmdlist *cmd)
{
    if (!cmd->av || !cmd->av[0])
        return;

    if (ft_strcmp(cmd->av[0], "echo") == 0
        || ft_strcmp(cmd->av[0], "pwd") == 0
        || ft_strcmp(cmd->av[0], "cd") == 0
        || ft_strcmp(cmd->av[0], "exit") == 0
        || ft_strcmp(cmd->av[0], "env") == 0
        || ft_strcmp(cmd->av[0], "export") == 0
        || ft_strcmp(cmd->av[0], "unset") == 0)
        cmd->is_builtin = 1;
    else
        cmd->is_builtin = 0;
}
void swap(char **a, char **b)
{
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

