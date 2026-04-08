#include "minishell.h"


static void sort_env(char **envp)
{
    int i;
    int j;

    i = 0;
    while (envp[i])
    {
        j = i + 1;
        while (envp[j])
        {
            if (strcmp(envp[i], envp[j]) > 0)
                swap(&envp[i], &envp[j]);
            j++;
        }
        i++;
    }
}

static void print_escaped(char *value)
{
    int i = 0;

    while (value[i])
    {
        if (value[i] == '"' || value[i] == '\\')
            write(1, "\\", 1);
        write(1, &value[i], 1);
        i++;
    }
}
static void print_env_var(char *env_var)
{
    char *eq;
    
    eq = ft_strchr(env_var, '=');

    write(1, "declare -x ", 11);
    if (eq)
    {
        write(1, env_var, eq - env_var);   
        write(1, "=\"", 2);
        print_escaped(eq + 1);             
        write(1, "\"\n", 2);
    }
    else
    {
        write(1, env_var, ft_strlen(env_var));
        write(1, "\n", 1);
    }
}

void print_export(char **envp)
{
    int i = 0;
    char **sorted;

    sorted = copy_envp(envp);
    if (!sorted)
        return ;
    sort_env(sorted);
    while (sorted[i])
    {
        print_env_var(sorted[i]);
        i++;
    }
    i = 0;
    while (sorted[i])
        free(sorted[i++]);
    free(sorted);
}
