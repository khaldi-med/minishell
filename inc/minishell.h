#ifndef MINISHELL_H
#define MINISHELL_H

#include "./libft/libft.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PATH_LEN 4096
#define SUCCESS 0
#define ERROR 1

typedef enum x_type {
  SINGLE_QUOTE, // '
  DOUBLE_QUOTE, // "
  INPUT,        // <
  OUTPUT,       // >
  HERE_DOC,     // <<
  APPEND,       // >>
  DOLLAR_SIGN,  // $
} t_type;

typedef struct x_red {
  char *file;
  t_type type;
  int check_flag;

} t_red;

typedef struct x_env {
  char *key;
  char *value;
  struct x_env *next;
} t_env;

typedef struct x_cmd {
  char *command;
  char **arguments;
  int check_red;
  int check_env;

  t_red **s_red;
  t_env *s_env;
  struct x_cmd *next;
} t_cmd;

typedef struct x_bash {
  char *commands;
  char **args_pip;
  int num_cmd;

  t_cmd **s_cmd;
  t_env *env_list;
  char **env_array;

} t_bash;

/*enviroment*/
char **ft_create_env_array(t_env *env_list);
t_env *ft_create_env_node(char *key, char *value);
int ft_add_env(t_env **env_list, char *key, char *value);
int ft_init_env(t_bash *bash, char **envp);
int ft_cleanup_env_array(char **env_array);
void ft_free_env_list(t_env *env_list);
#endif
