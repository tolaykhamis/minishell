

#include "minishell.h"

char    **copy_envp(char **envp)
{
    int     i;
    char    **new_envp;

    i = 0;
    while (envp[i])
        i++;
    new_envp = ft_calloc(i + 1, sizeof(char *));
    if (!new_envp)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        new_envp[i] = ft_strdup(envp[i]);
        if (!new_envp[i])
        {
            while (i--)
                free(new_envp[i]);
            free(new_envp);
            return (NULL);
        }
        i++;
    }
    return (new_envp);
}
static void process_line(char *line, t_shell *shell)
{
    t_token     *tokens;
    t_cmdlist   *cmds;

    tokens = NULL;
    cmds = NULL;
    extract_token_list(line, &tokens);
    if (!tokens)
        return ;
    if (!check_syntax(tokens))
    {
        shell->exit_status = 2;
        free_tokens(tokens);
        return ;
    }
    argumentssloop(tokens, &cmds);
    expander(&cmds, shell);
    shell->cmds = cmds;
    // debug_commands(shell->cmds);
    // debug_tokens(tokens);
    execute_command(shell);
    free_cmds(cmds);
    shell->cmds = NULL;
    free_tokens(tokens);
}

static void shell_loop(t_shell *shell)
{
    char *line;

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break ;
        if (g_signal == SIGINT)
        {
            shell->exit_status = 130;
            g_signal = 0;
        }
        if (*line)
        {
            add_history(line);
            process_line(line, shell);
        }
        free(line);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_shell *shell;

    (void)argc;
    (void)argv;
    shell->envp = copy_envp(envp);
    shell->export = copy_envp(envp);
    shell->exit_status = 0;
    shell->cmds = NULL;
    setup_signals_interactive();
    shell_loop(shell);
    cleanup_shell(shell);
    free_free(shell->envp);
    free_cmds(shell->cmds);
    rl_clear_history();
    return (shell.exit_status);
}
