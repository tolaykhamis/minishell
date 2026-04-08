#include "minishell.h"

char *command_path(char *cmd, char **env,int i)
{
    char    *path_env;
    char    **all_paths;
    char    *tmp;
    char    *full_path;

    path_env = get_env_name(env, "PATH");
    if (!path_env)
        return NULL;
    all_paths = ft_split(path_env, ':');
    while (all_paths && all_paths[i])
    {
        tmp = ft_strjoin(all_paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (!access(full_path, X_OK))
        {
            free_free(all_paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_free(all_paths);
    return NULL;
}

static void go_child(char *path, t_cmdlist *cmd, t_shell *shell)
{
    execve(path, cmd->av, shell->envp);
    perror(cmd->av[0]);
    exit(126);
}

static void wait_ya_parent(pid_t pid, int *status, t_shell *shell)
{
    waitpid(pid, status, 0);
    if (WIFEXITED(*status))
        shell->exit_status = WEXITSTATUS(*status);
    else if (WIFSIGNALED(*status))
        shell->exit_status = 128 + WTERMSIG(*status);
}
static void family_time(pid_t pid, t_shell *shell,t_cmdlist *cmd,char *path)
{
    int status;
    if (pid == 0)
        go_child(path, cmd, shell);
    else if(pid > 0)
        wait_ya_parent(pid, &status, shell);
    else 
    {
        perror("fork");
        shell->exit_status = 1;
    }
}
void execute_external(t_shell *shell,t_cmdlist *cmd)
{
    pid_t pid;
    char *path;

    if (ft_strchr(cmd->av[0], '/'))
        path = cmd->av[0];
    else
        path = command_path(cmd->av[0], shell->envp,0);
    if (!path)
    {
        put_str_fd("minishell: ", 2);
        put_str_fd(cmd->av[0], 2);
        put_str_fd(": command not found\n", 2);
        shell->exit_status = 127;        
        return ;
    }
    pid = fork();
    family_time(pid,shell,cmd,path);
    if (path != cmd->av[0])
        free(path);
}

