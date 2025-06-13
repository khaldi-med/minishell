#include "../inc/minishell.h"

char **ft_create_env_array(t_env *env_list) {
  t_env *current;
  char **env_array;
  int count;
  int i;
  char *tmp;

  if (!env_list)
    return (NULL);
  count = 0;
  current = env_list;
  while (current) {
    count++;
    current = current->next;
  }
  env_array = (char **)malloc(sizeof(char *) * (count + 1));
  if (!env_array)
    return (NULL);
  i = 0;
  current = env_list;
  while (current) {
    tmp = malloc(ft_strlen(current->key) + ft_strlen(current->value) + 2);
    if (!tmp) {
      while (--i >= 0)
        free(env_array[i]);
      free(env_array);
      return (NULL);
    }
    env_array[i++] = tmp;
    current = current->next;
  }
  env_array = NULL;
  return (env_array);
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
