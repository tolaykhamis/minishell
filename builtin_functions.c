#include "minishell.h"


static char **remove_from_envp(char **envp, char *name)
{
    int     i;
    int     j;
    int     len;
    char    **new_envp;

    len = ft_strlen(name);
    i = 0;
    while (envp[i])
        i++;
    new_envp = ft_calloc(i + 1, sizeof(char *));
    if (!new_envp)
        return (envp);
    i = 0;
    j = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            free(envp[i]);  
        else
            new_envp[j++] = envp[i]; 
        i++;
    }
    new_envp[j] = NULL;
    free(envp); 
    return (new_envp);
}
static int  is_valid_identifier(char *str)
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
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
static char **add_to_envp(char **envp, char *new)
{
    int     i;
    int     len;
    char    **new_envp;

    len = 0;
    i = 0;
    while (new[len] && new[len] != '=')
        len++;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], new, len) && envp[i][len] == '=')
        {
            free(envp[i]);
            envp[i] = ft_strdup(new); 
            return (envp);
        }
        i++;
    }
    new_envp = ft_calloc(i + 2, sizeof(char *));
    if (!new_envp)
        return (envp);
    i = 0;
    while (envp[i])
    {
        new_envp[i] = envp[i];
        i++;
    }
    new_envp[i] = ft_strdup(new);
    new_envp[i + 1] = NULL;
    free(envp);
    return (new_envp);
}



int execute_builtin(t_shell *shell,t_cmdlist *cmd)
{
    char pwd_output[1024];
    int i;
    int j;

    i = 1;
    if (ft_strcmp(cmd->av[0], "cd") == 0)
    {
        char *path;
    
        if (!cmd->av[1])
            path = getenv("HOME");
        else
            path = cmd->av[1];
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
        int n = 1;
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
    else if (ft_strcmp(cmd->av[0], "exit") == 0)
    {
        int code;
        
        code = shell->exit_status;
        if (cmd->av[1])
        {
            if (cmd->av[2])
            {
                put_str_fd("minishell: exit: too many arguments\n", 2);
                return (1);
            }
            j = 0;
            if (cmd->av[1][0] == '-' || cmd->av[1][0] == '+')
                j++;
            while (cmd->av[1][j])
            {
                if (cmd->av[1][j] < '0' || cmd->av[1][j] > '9')
                {
                    put_str_fd("minishell: exit: ", 2);
                    put_str_fd(cmd->av[1], 2);
                    put_str_fd(": numeric argument required\n", 2);
                    exit(2);
                }
                j++;
            }
        code = ft_atoi(cmd->av[1]) % 256;
        }
        exit(code);
    }
    else if (ft_strcmp(cmd->av[0], "export") == 0)
    {
       if (!cmd->av[1])
        {
            i = 0;
            while (shell->envp[i])
            {
                put_str_fd(shell->envp[i], 1);
                put_str_fd("\n", 1);
                i++;
            }
            return (0);
        }
        i = 1;
        while (cmd->av[i])
        {
            if (!is_valid_identifier(cmd->av[i]))
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
    else if (ft_strcmp(cmd->av[0], "unset") == 0)
    {
        i = 1;
        if (!cmd->av[1])
            return(0);
        while (cmd->av[i])
        {
            shell->envp = remove_from_envp(shell->envp, cmd->av[i]);
            i++;
        }    
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


