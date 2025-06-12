#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_PATH_LEN 4096
# define SUCCESS 0
# define ERROR 1

typedef enum x_type
{
	// REDIRECTION,
	// ENVIROMENT,
	// SINGLE_QUOTE, // '
	// DOUBLE_QUOTE, // "
	INPUT,       // <
	OUTPUT,      // >
	HERE_DOC,    // <<
	APPEND,      // >>
	DOLLAR_SIGN, // $
}					t_type;

typedef struct x_red
{
	char			*file;
	t_type			type;
	int				check_flag;

}					t_red;

typedef struct x_env
{
	char			*key;
	char			*value;
	struct x_env	*next;
}					t_env;

typedef struct x_cmd
{
	char			*command;
	char			**arguments;
	int				check_red;
	int				check_env;

	t_red			**s_red;
	t_env			*s_env;
	struct x_cmd	*next;
}					t_cmd;

typedef struct x_bash
{
	char			*commands;
	char			**args_pip;
	int				num_cmd;

	t_cmd			**s_cmd;
	t_env			*env_list;
	char			**env_array;

}					t_bash;

/*
 * Environment management
 */
char				*get_env_value(t_env *env_list, char *key);
int					update_env(t_bash *bash, char *key, char *value);
int					init_env(t_bash *bash, char **envp);
int					add_env(t_env **env_list, char *key, char *value);
char				**create_env_array(t_env *env_list);
void				free_env_array(char **env_array);
void				free_env_list(t_env *env_list);

//->parsing:

int					select_struct1(t_bash *bash);
int					select_struct2(t_bash *bash);
int					select_struct3(t_bash *bash);
void				select_struct(t_bash *bash);

//->parsing1:
int					check_cmd(char *str);
int					check_cmd1(char *str);
int					count_pipes(char *cmd);
int					red_parse(t_bash *bash);

//->parsing2:

int					check_redirection(char *str);
int					check_envirment(char *str);
void				check_red_env(t_bash *bash);

//->tokrnizer

char				**tokenizer(char *cmd);
int					check_tokinzer(char c);
int					count_word_tk(char *cmd);
void				free_double_pointer_tk(char **arr, int p);
char				**alloc_words_tk(char **ret, char *cmd, int word);

/*
 * Signal handling
 */
void				active_signal(void);
void				handel_sigint(int sig);
void				handel_sigquit(int sig);

/*
 * Builtin functions
 */
int					builtin_echo(t_cmd *cmd);
int					builtin_cd(t_bash *bash, t_cmd *cmd);
int					builtin_pwd(void);
int					builtin_export(t_bash *bash, t_cmd *cmd);
int					builtin_unset(t_bash *bash, t_cmd *cmd);
int					builtin_env(t_bash *bash);
int					builtin_exit(t_bash *bash, t_cmd *cmd);

/*
 * Execution utilities
 */
void				free_array(char **arr);
int					apply_redirections(t_cmd *cmd);
void				restore_redirections(void);
int					execute_pipeline(t_bash *bash, int *exit_status);
int					execute_command(t_bash *bash, t_cmd *cmd, int *exit_status);
int					execute_commands(t_bash *bash);
int					is_builtin(char *cmd);
int					execute_builtin(t_bash *bash, t_cmd *cmd, int *exit_status);
char				*find_command_path(t_bash *bash, char *cmd);

/*
 * Environment variable expansion
 */
char				*expand_variables(t_bash *bash, char *str);
void				expand_command_variables(t_bash *bash, t_cmd *cmd);

/*
 * Parsing and cleanup utilities
 */
int					parse_input(t_bash *bash, char *input);
void				free_redirection(t_red **red);
void				remove_redirection_from_args(t_cmd *cmd);
void				setup_parent_signals(void);
void				setup_child_signals(void);
void				free_array(char **arr);
#endif
