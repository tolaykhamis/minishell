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
        i++;
    }
    return (new_envp);
}
int main(int argc, char **argv, char **envp)
{
    t_token     *tokens;
    t_cmdlist   *cmds;
    t_shell     shell;
    char        *line;

    (void)argc; (void)argv;
    shell.envp = copy_envp(envp);
    shell.exit_status = 0;
    setup_signals_interactive();

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (g_signal == SIGINT)
        {
            shell.exit_status = 130;
            g_signal = 0;
        }
        if (*line)
        {
            add_history(line);
            tokens = NULL;
            cmds = NULL;
            extract_token_list(line, &tokens);
            if (tokens) 
            {
                if (!check_syntax(tokens))
                {
                    shell.exit_status = 258;
                    free_tokens(tokens);
                }
                else
                {
                    argumentssloop(tokens, &cmds);
                    expander(&cmds, &shell);
                    shell.cmds = cmds;          
                    // debug_tokens(tokens);
                    // debug_commands(cmds);
                    // debug_pipeline(cmds);
                    execute_command(&shell);
                    free_cmds(cmds);
                    shell.cmds = NULL;          
                    free_tokens(tokens);
                }
            }
        }
        free(line);
    }
    rl_clear_history();
    return (shell.exit_status);
}
