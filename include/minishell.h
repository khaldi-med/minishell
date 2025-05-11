#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "minishell$ "
# define MAX_PATH_LEN 4096
# define MAX_ARGS 256

/*
 * Token types for the lexer
 */
typedef enum e_token_type
{
	TOKEN_WORD,         // Command or argument
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_AMPERSAND,    // &
	TOKEN_SEMICOLON,    // ;
	TOKEN_AND,          // &&
	TOKEN_OR,           // ||
	TOKEN_NEWLINE,      // \n
	TOKEN_EOF           // End of input
}						t_token_type;

/*
 * Token structure for the lexer
 */
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

/*
 * Command structure representing a simple command
 */
typedef struct s_command
{
	char				**args;
	int					input_fd;
	int					output_fd;
	int					is_builtin;
}						t_command;

/*
 * Pipeline structure representing a sequence of commands
 */
typedef struct s_pipeline
{
	t_command			*commands;
	int					cmd_count;
	struct s_pipeline	*next;
}						t_pipeline;

/*
 * Environment variables linked list
 */
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

/*
 * Main shell structure
 */
typedef struct s_shell
{
	t_env				*env;
	t_token				*tokens;
	t_pipeline			*pipeline;
	int					exit_status;
	int					running;
}						t_shell;

/*
 * Function prototypes
 */

/* Initialization and cleanup */
t_shell					*init_shell(char **envp);
void					free_shell(t_shell *shell);

/* Lexer functions */
t_token					*tokenize_input(char *input);
void					free_tokens(t_token *tokens);

/* Parser functions */
int						parse_tokens(t_shell *shell);
void					free_pipeline(t_pipeline *pipeline);

/* Execution functions */
int						execute_pipeline(t_shell *shell);
int						execute_command(t_shell *shell, t_command *cmd);

/* Builtin commands */
int						is_builtin(char *cmd);
int						execute_builtin(t_shell *shell, t_command *cmd);

/* Environment variable functions */
t_env					*init_env(char **envp);
char					*get_env_value(t_env *env, const char *key);
void					set_env_value(t_env *env, const char *key,
							const char *value);
void					free_env(t_env *env);

/* Signal handling */
void					setup_signals(void);
void					reset_signals(void);

/* Utility functions */
void					display_prompt(void);
char					*expand_variables(t_shell *shell, char *str);
char					*find_command_path(t_shell *shell, char *cmd);
void					handle_error(char *msg, int exit_code);

#endif
