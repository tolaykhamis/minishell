#include "minishell.h"

int execute_builtin(t_shell *shell,t_cmdlist *cmd)
{
    char pwd_output[1024];
    int i;

    i = 1;
    if (ft_strcmp(cmd->av[0], "cd") == 0)
    {
        char *path;
    
        if (!cmd->av[1])
            path = getenv("HOME");
        else
        path = shell->cmds->av[1];
        if (chdir(path) != 0)
        {
            put_str_fd("minishell: cd: ", 2);
            put_str_fd(path, 2);
            put_str_fd(": No such file or directory\n", 2);
            return (1);
        }
        return (0);
    }
    else if (ft_strcmp(cmd->av[0], "pwd") == 0)
    {
        if (getcwd(pwd_output, sizeof(pwd_output)))
        {
            put_str_fd(pwd_output, 1);
            put_str_fd("\n", 1);
            return (0);
        }
        return (1);
    }
    else if (ft_strcmp(cmd->av[0], "echo") == 0)
    {
        int newline = 1;
        i = 1;
        if (cmd->av[1] && ft_strcmp(cmd->av[1], "-n") == 0)
        {
            newline = 0;
            i = 2;
        }
        while (cmd->av[i])
        {
            put_str_fd(cmd->av[i], 1);
            if (cmd->av[i + 1])
                put_str_fd(" ", 1);
            i++;
        } 
        if (newline)
        put_str_fd("\n", 1);
        return (0);
    }
    else if (ft_strcmp(cmd->av[0], "exit") == 0)
    {
        int code = shell->exit_status;
        if (cmd->av[1])
        code = ft_atoi(cmd->av[1]);
        put_str_fd("exit\n", 1);
        exit(code);
    }
    else if (ft_strcmp(cmd->av[0], "export") == 0)
    {
        if (!cmd->av[1])
        {
            put_str_fd("export needs argument\n",1);
            return(1);
        }
        putenv(cmd->av[1]);
        return (0);
    }
    else if (ft_strcmp(cmd->av[0], "unset") == 0)
    {
        if (!cmd->av[1])
            return(1);
        unsetenv(cmd->av[1]);
        return (0);
    }
    else if (ft_strcmp(cmd->av[0], "env") == 0)
    {
        i = 0;
        while (shell->envp[i])
        {
            put_str_fd(shell->envp[i],1);
            put_str_fd("\n",1);
            i++;
        }
        return (0);
    }
    return (-1);
}


