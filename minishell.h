#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

enum token
{
	TOKEN_WORD,
	TOKENPIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_HERDOC,
	INFILE,
	OUTFILE,
	ARGUMENT
};

typedef struct s_token
{
	char *value;
	enum token type;
	struct s_token *next;
} t_token;

typedef struct s_redi
{
	enum token		type;
	char			*f;
    int             has_quotes;
	struct s_redi	*next;
	int				heredoc_fd;
} t_redi;

typedef struct s_cmdlist
{
	char				**av;
	t_redi				*redirs;
	struct s_cmdlist	*next;
	int					is_builtin;
} t_cmdlist;

typedef struct s_shell
{
	char		**envp;
	char		**export;
	int			exit_status;
	t_cmdlist	*cmds;
} t_shell;

extern int g_signal;
void add_redi_node(t_cmdlist *cmd, t_token *token);
char    *ft_strchr(const char *s, int c);
char    *strip_quotes(char *str);
void		extract_token_list(char *line, t_token **token_list);
int			check_syntax(t_token *tokens);
int			outputerror(const char *token);
void		argumentssloop(t_token *tokens, t_cmdlist **cmds);
void	expander(t_cmdlist **cmds, t_shell *shell);
char		*expand_parameter(char *str, t_shell *shell);
char		*extract_var_name(char *str, int *i);
char		*getval(const char *name, t_shell *shell);
char		*append_str(char *res, const char *add);
char		*append_char(char *res, char c);
void		freeeeeeeeeeeeeee(t_cmdlist *cmds);
void		free_tokens(t_token *tokens);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_itoa(int n);
size_t		ft_strlen(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char	**ft_split(char const *s, char c);
char		*ft_strdup(const char *s);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_isalnum(int c);
void		debug_commands(t_cmdlist *cmds);
void		debug_pipeline(t_cmdlist *cmds);
void		debug_tokens(t_token *tokens);
char	*substr_dup(char *start, int len);
int	word_length(char *ptr);
void	handle_word(char **ptr, t_token **list);
void	handle_input(char **ptr, t_token **list);
void	handle_output(char **ptr, t_token **list);
void	pipes(char **ptr, t_token **list);
void	handle_operator(char **ptr, t_token **list);
void addnode_redi(t_cmdlist *cmd, t_token *token);
void	handle_word(char **ptr, t_token **list);
int	count_args(t_token *tokens);
void free_redirs(t_redi *redi);
void free_cmds(t_cmdlist *cmds);
int	is_redir(enum token type);
int	is_operator(char c);
int	ft_space(char c);
t_token	*new_token(char *value, enum token type);
void	add_token(t_token **list, t_token *new);
char *quote(char **ptr);
int	valoftoken(t_token *t);
int	redi(enum token type);
int	oper(enum token type);
char	*convert(enum token type);
int execute_builtin(t_shell *shell,t_cmdlist *cmd);
void execute_command(t_shell *shell);
void single_execution(t_shell *shell,t_cmdlist *cmd);
void    pipeline_execution(t_shell *shell,int i);
void execute_external(t_shell *shell,t_cmdlist *cmd);
void init_isbuiltin(t_cmdlist *cmd);
char	*ft_strjoin(char  *s1, char  *s2);
char *get_path_from_env(char **env);
char	*ft_strchr(const char *s, int c);
void	put_str_fd(char *s, int fd);
int	ft_atoi(const char *str);
void	handle_cmds(t_token *tokens, t_cmdlist **cmds);
int     apply_redirections(t_cmdlist *cmd, t_shell *shell);
void setup_signals_interactive(void);
void setup_signals_child(void);
void setup_signals_heredoc(void);
int builtin_cd(t_shell *shell, t_cmdlist *cmd);
char **remove_from_envp(char **envp, char *name);
char **add_to_envp(char **envp, char *new);
char *get_env_name(char **envp, char *name);
int  builtin_echo(t_cmdlist *cmd);
void update_pwd(t_shell *shell);
int  builtin_pwd(void);
int  builtin_exit(t_shell *shell, t_cmdlist *cmd);
int  builtin_env(t_shell *shell);
int is_numeric(char *s);
void free_free(char **arr);
char **takeword(char *str, t_shell *shell);
void	free_i(char **res, int i);
char	*word_dup(const char *s, size_t len);
int	count_words(const char *s, char c);
char	*quote_removal_part_100(char *str);
char *command_path(char *cmd, char **env,int i);
const char *token_name(enum token type);
int	fill(char **res, const char *s, char c);
int apply_heredoc(t_redi *rd, t_shell *shell);
void swap(char **a, char **b);
void print_export(char **envp);
char    **copy_envp(char **envp);
void wait_all(t_shell *shell, pid_t *pids, int count);
pid_t    *init_pids(int count);
int  cmds_len(t_cmdlist *cmds);
int yes_value(char *str);
int preprocess_heredocs(t_cmdlist *cmds, t_shell *shell);


#endif
