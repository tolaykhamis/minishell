#include "minishell.h"

void	debug_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	printf("\n===== TOKENS =====\n");
	while (tokens)
	{
		printf("[%d] type=%s  value=\"%s\"\n", i, token_name(tokens->type),
			tokens->value);
		tokens = tokens->next;
		i++;
	}
	printf("==================\n\n");
}

void	debug_commands(t_cmdlist *cmds)
{
	int		i;
	int		cmd_count;
	t_redi	*r;

	cmd_count = 0;
	printf("\n=== COMMAND TABLE DEBUG ===\n");
	while (cmds)
	{
		printf("Command [%d]:\n", cmd_count);
		if (cmds->av)
		{
			i = 0;
			while (cmds->av[i])
			{
				printf("  av[%d] = \"%s\"\n", i, cmds->av[i]);
				i++;
			}
		}
		r = cmds->redirs;
		while (r)
		{
			printf("  redir: type = %s, file = \"%s\"\n", convert(r->type),
				r->f);
			r = r->next;
		}
		cmds = cmds->next;
		cmd_count++;
		printf("---------------------------\n");
	}
	printf("=============================\n\n");
}

void	debug_pipeline(t_cmdlist *cmds)
{
	printf("\nPIPELINE VIEW:\n");

	while (cmds)
	{
		if (cmds->av)
			printf("[%s]", cmds->av[0]);

		if (cmds->next)
			printf("  |  ");

		cmds = cmds->next;
	}

	printf("\n\n");
}