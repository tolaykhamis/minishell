#include "minishell.h"

char **remove_from_envp(char **envp, char *name)
{
    int i;
    int j;
    int len;

    if (!envp || !name)
        return envp;
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0)
        {
            free(envp[i]);
            j = i;
            while (envp[j])
            {
                envp[j] = envp[j + 1];
                j++;
            }
            return envp;
        }
        i++;
    }
    return envp;
}
static int update_existing(char **envp, char *new)
{
    int i = 0;
    int len = 0;

    while (new[len] && new[len] != '=')
        len++;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], new, len))
        {
            free(envp[i]);
            envp[i] = ft_strdup(new);
            return (1);
        }
        i++;
    }
    return (0);
}
static char **add_new(char **envp, char *new)
{
    int i = 0;
    char **new_envp;

    while (envp[i])
        i++;

    new_envp = ft_calloc(i + 2, sizeof(char *));
    if (!new_envp)
        return envp;

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
char **add_to_envp(char **envp, char *new)
{
    if (!envp || !new)
        return envp;

    if (update_existing(envp, new))
        return (envp);

    return add_new(envp, new);
}

char *get_env_name(char **envp, char *name)
{
    int     i;
    int     len;

    if (!name || !envp)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}
