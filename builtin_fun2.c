#include "minishell.h"

int  builtin_echo(t_cmdlist *cmd)
{
    int     n;
    int     i;

    n = 1;
    i = 1;
    if (cmd->av[1] && ft_strcmp(cmd->av[1], "-n") == 0)
    {
        n = 0;
        i = 2;
    }
    while (cmd->av[i])
    {
        put_str_fd(cmd->av[i], 1);
        if (cmd->av[i + 1])
            put_str_fd(" ", 1);
        i++;
    }
    if (n)
        put_str_fd("\n", 1);
    return (0);
}

int is_numeric(char *s)
{
    int i;
    
    i = 0;
    if (!s)
        return 0;
    if (s[0] == '-' || s[0] == '+')
        i++;
    while (s[i])
    {
        if (s[i++] < '0' || s[i-1] > '9')
            return 0;
    }
    return 1;
}

int builtin_exit(t_shell *shell, t_cmdlist *cmd)
{
    int code;

    put_str_fd("exit\n", 1);
    if (!cmd->av[1])
        exit(shell->exit_status);
    if (cmd->av[2])
    {
        put_str_fd("minishell: exit: too many arguments\n", 2);
        return (1);
    }
    if (!is_numeric(cmd->av[1]))
    {
        put_str_fd("minishell: exit: ", 2);
        put_str_fd(cmd->av[1], 2);
        put_str_fd(": numeric argument required\n", 2);
        exit(2);
    }
    code = ft_atoi(cmd->av[1]) % 256;
    exit(code);
}

int  builtin_env(t_shell *shell)
{
    int     i;

    i = 0;
    while (shell->envp[i])
    {
        put_str_fd(shell->envp[i], 1);
        put_str_fd("\n", 1);
        i++;
    }
    return (0);
}
