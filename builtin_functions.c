#include "minishell.h"

static int  is_valid_id(char *str)
{
    int i;

    if (!str || str[0] == '\0')
        return (0);
    if (!((str[0] >= 'a' && str[0] <= 'z')
        || (str[0] >= 'A' && str[0] <= 'Z')
        || str[0] == '_'))
        return (0);
    i = 1;
   while (str[i] && str[i] != '=')
    {
        if (!(
            (str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= '0' && str[i] <= '9') ||
            str[i] == '_'
        ))
        return (0);
        i++;
    }
return (1);
}


static int  builtin_export(t_shell *shell, t_cmdlist *cmd)
{
    int     i;

    if (!cmd->av[1])
       builtin_env(shell);

    i = 1;
    while (cmd->av[i])
    {
        if (!is_valid_id(cmd->av[i]))
        {
            put_str_fd("minishell: export: `", 2);
            put_str_fd(cmd->av[i], 2);
            put_str_fd("': not a valid identifier\n", 2);
            return (1);
        }
        shell->envp = add_to_envp(shell->envp, cmd->av[i]);
        i++;
    }
    return (0);
}

static int  builtin_unset(t_shell *shell, t_cmdlist *cmd)
{
    int     i;

    if (!cmd->av[1])
        return (0);
    i = 1;
    while (cmd->av[i])
    {
        shell->envp = remove_from_envp(shell->envp, cmd->av[i]);
        i++;
    }
    return (0);
}



int execute_builtin(t_shell *shell, t_cmdlist *cmd)
{
    if (ft_strcmp(cmd->av[0], "cd") == 0)
        return (builtin_cd(shell, cmd));
    if (ft_strcmp(cmd->av[0], "pwd") == 0)
        return (builtin_pwd());
    if (ft_strcmp(cmd->av[0], "echo") == 0)
        return (builtin_echo(cmd));
    if (ft_strcmp(cmd->av[0], "exit") == 0)
        return (builtin_exit(shell, cmd));
    if (ft_strcmp(cmd->av[0], "export") == 0)
        return (builtin_export(shell, cmd));
    if (ft_strcmp(cmd->av[0], "unset") == 0)
        return (builtin_unset(shell, cmd));
    if (ft_strcmp(cmd->av[0], "env") == 0)
        return (builtin_env(shell));
    return (-1);
}