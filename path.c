
#include "minishell.h"

char *get_path_from_env(char **env)
{
    int i = 0;

    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
            return env[i] + 5;
        i++;
    }
    return NULL;
}

char *command_path(char *cmd, char **env)
{
    char    *path_env;
    char    **paths;
    char    *tmp;
    char    *full_path;
    int     i;

    path_env = get_path_from_env(env);
    if (!path_env)
        return NULL;
    paths = ft_split(path_env, ':');
    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
            return full_path;
        free(full_path);
        i++;
    }
    return NULL;
}
