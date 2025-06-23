#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/utils/libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define MAX_PATH 1024
# define MAX_ARGS 100

/* Global variables */
extern int			g_signal;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	char			**env;
	int				exit_status;
	t_cmd			*cmds;
}					t_shell;

/* Executor functions */
void				ft_exec_cmds(t_shell *shell, t_cmd *cmds);
void				ft_exec_sing_cmd(t_shell *shell, t_cmd *cmd);
void				ft_exec_pipe(t_shell *shell, t_cmd *cmds);
void				ft_exec_external_cmd(t_cmd *cmd, char **env);
int					ft_setup_redirections(t_redir *redirs);

/* Built-in commands */
int					ft_is_builtin(char *cmd);
int					ft_exec_builtin(t_shell *shell, char **args);
int					ft_builtin_echo(char **args);
int					ft_builtin_cd(char **args);
int					ft_builtin_pwd(void);
int					ft_builtin_export(t_shell *shell, char **args);
int					ft_builtin_unset(t_shell *shell, char **args);
int					ft_builtin_env(t_shell *shell);
int					ft_builtin_exit(t_shell *shell, char **args);

/* Environment functions */
char				*ft_get_env_value(char **env, char *key);
void				ft_set_env_value(t_shell *shell, char *key, char *value);
void				ft_unset_env_value(t_shell *shell, char *key);
char				**ft_copy_env(char **env);

/* Expansion functions */
char				*ft_expand_variables(char *str, t_shell *shell);
char				*ft_expand_dollar(char *str, int *i, t_shell *shell);
char				*ft_get_variable_value(char *var, t_shell *shell);

/* Quote handling */
char				*ft_handle_quotes(char *str);
char				*ft_remove_quotes(char *str);
int					ft_is_quoted(char *str, int pos);

/* Path resolution */
char				*ft_find_executable(char *cmd, char **env);
char				**ft_get_path_dirs(char **env);
int					ft_is_executable(char *path);

/* Utility		functions */
char				**ft_split_args(char *str);

/* Heredoc functions */
void				ft_handle_heredoc(char *delimiter);
int					ft_setup_heredoc(char *delimiter);
int					ft_preprocess_heredocs(t_cmd *cmds);
int					ft_process_heredoc_input(char *delimiter);

/* Signal functions */
void				ft_setup_signals(void);
void				ft_signal_handler(int sig);
void				ft_handle_sigint(void);
void				ft_handle_sigquit(void);

/* Lexer and parser functions */
t_token				*ft_tokenize(char *input);
t_token				*ft_creat_token(char *value, t_token_type type);
void				ft_add_token(t_token **tokens, t_token *new_token);
t_token				*ft_handle_redirection(char *input, int *i);
int					ft_skip_space(char *input, int i);
t_cmd				*ft_parse_tokens(t_token *tokens, t_shell *shell);
void				ft_free_tokens(t_token *tokens);
void				ft_free_cmds(t_cmd *cmds);

/* Parse utility functions */
void				ft_process_word_token(t_cmd *cmd, t_token *token, int *index,
						t_shell *shell);
int					ft_count_args(t_token *tokens);
void				ft_process_redir_token(t_cmd *cmd, t_token **token, t_shell *shell);

/* Command creation functions */
t_cmd				*ft_creat_cmd(void);
void				ft_add_redir(t_cmd *cmd, t_token_type type, char *file);

/* Shell management functions */
void				ft_init_shell(t_shell *shell, char **envp);
void				ft_cleanup_shell(t_shell *shell);
void				ft_shell_loop(t_shell *shell);

// free functions
void				ft_free_path_dirs(char **path_dirs);
#endif
