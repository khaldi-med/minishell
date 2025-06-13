#include "inc/minishell.h"

int exit_status;

int ft_error(char *s) {
  perror(s);
  return (1);
}

static void ft_cleanup_env_list(t_env *env_list) {
  t_env *current;
  t_env *next;

  current = env_list;
  while (current) {
    next = current->next;
    free(current->key);
    free(current->value);
    free(current);
    current = next;
  }
}

t_env *ft_create_env_node(char *key, char *value) {
  t_env *new_env;

  new_env = (t_env *)malloc(sizeof(t_env));
  if (!new_env)
    return (NULL);
  new_env->key = ft_strdup(key);
  if (!new_env->key) {
    free(new_env);
    return (NULL);
  }
  new_env->key = ft_strdup(value);
  if (!new_env->value) {
    free(new_env->key);
    free(new_env);
    return (NULL);
  }
  new_env->next = NULL;
  return (new_env);
}

int ft_add_env(t_env **env_list, char *key, char *value) {
  t_env *new_env;
  t_env *current;
  char *new_value;

  if (!env_list || !key || !value)
    return (ERROR);
  current = *env_list;
  while (current) {
    if (ft_strcmp(current->key, key) == 0) {
      new_value = ft_strdup(value);
      if (!new_value)
        return (ERROR);
      free(current->value);
      current->value = new_value;
      return (SUCCESS);
    }
    current = current->next;
  }
  new_env = ft_create_env_node(key, value);
  if (!new_env)
    return (ERROR);
  new_env->next = *env_list;
  *env_list = new_env;
  return (SUCCESS);
}

int ft_init_env(t_bash *bash, char **envp) {
  int i;
  char *key;
  char *value;
  char *equal_sing;

  if (!bash || !envp || !envp[0])
    return (ERROR);
  bash->env_list = NULL;
  i = 0;
  while (envp[i]) {
    equal_sing = ft_strchr(envp[i], '=');
    if (!equal_sing) {
      i++;
      continue;
    }
    *equal_sing = '\0';
    key = envp[i];
    value = equal_sing + 1;
    if (ft_add_env(&(bash->env_list), key, value) == ERROR) {
      *equal_sing = '=';
      ft_cleanup_env_list(bash->env_list);
      bash->env_list = NULL;
    }
    *equal_sing = '=';
    i++;
  }
  if (!bash->env_list)
    return (ERROR);
  return (SUCCESS);
}

int ft_init_shell(t_bash *bash, char **envp) {
  if (!bash || !envp)
    return (ERROR);
  bash->env_list = NULL;
  bash->env_array = NULL;
  bash->s_cmd = NULL;
  bash->commands = NULL;
  bash->args_pip = NULL;
  bash->num_cmd = 0;
  if (ft_init_env(bash, envp) == ERROR)
    ft_error("failed to initialize envirnoment\n");
  bash->env_array = ft_create_env_array(bash->env_list);
  if (!bash->env_array) {
    perror("minishell: failed to create envirnoment array\n");
    ft_free_env_list(bash->env_list);
    bash->env_list = NULL;
    return (ERROR);
  }
  return (SUCCESS);
}

void ft_shell_loop(t_bash *bash) {
  char *line;
  while (1) {
    line = readline("minishell> ");
    if (!line) {
      ft_error("minishell: readline failed");
      break;
    }
    if (*line) {
      add_history(line);
      parse_input(bash, )
    }
    free(line);
  }
}

int main(int argc, char **argv, char **envp) {
  t_bash *bash;
  (void)argc;
  (void)argv;
  bash = malloc(sizeof(t_bash));
  if (!bash)
    return (EXIT_FAILURE);
  if (ft_init_shell(bash, envp) == ERROR) {
    free(bash);
    return (EXIT_FAILURE);
  }
  ft_shell_loop(bash);
  ft_cleanup_shell(bash);
  return (exit_status);
}
