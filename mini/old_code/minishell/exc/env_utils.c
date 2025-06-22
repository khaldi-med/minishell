#include "../inc/minishell.h"

// Forward declarations for missing functions
char *ft_strchr(const char *str, int c);

// Global environment list
t_env *g_env_list = NULL;

// Function declarations
int add_env(t_env **env_list, char *key, char *value);
int delete_env(t_env **env_list, char *key);

// Environment functions
/*
 * Create a new environment variable node
 */
t_env *create_env_node(char *key, char *value) {
  t_env *new_env;

  new_env = (t_env *)malloc(sizeof(t_env));
  if (!new_env)
    return (NULL);

  new_env->key = ft_strdup(key);
  if (!new_env->key) {
    free(new_env);
    return (NULL);
  }

  new_env->value = ft_strdup(value);
  if (!new_env->value) {
    free(new_env->key);
    free(new_env);
    return (NULL);
  }

  new_env->next = NULL;
  return (new_env);
}

/*
 * Initialize environment variables from system environment
 */
static void cleanup_env_list(t_env *env_list) {
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

int init_env(t_bash *bash, char **envp) {
  int i;
  char *key;
  char *value;
  char *equal_sign;

  if (!bash || !envp || !envp[0])
    return (ERROR);

  bash->env_list = NULL;
  i = 0;
  while (envp[i]) {
    equal_sign = ft_strchr(envp[i], '=');
    if (!equal_sign) {
      i++;
      continue;
    }

    *equal_sign = '\0';
    key = envp[i];
    value = equal_sign + 1;

    if (add_env(&(bash->env_list), key, value) == ERROR) {
      *equal_sign = '=';
      cleanup_env_list(bash->env_list);
      bash->env_list = NULL;
      return (ERROR);
    }
    *equal_sign = '=';
    i++;
  }

  if (!bash->env_list)
    return (ERROR);

  g_env_list = bash->env_list;
  return (SUCCESS);
}
/*
 * Get value of an environment variable
 */
char *get_env_value(t_env *env_list, char *key) {
  t_env *current;

  if (!env_list || !key)
    return (NULL);

  current = env_list;
  while (current) {
    if (ft_strcmp(current->key, key) == 0)
      return (current->value);
    current = current->next;
  }

  return (NULL);
}

/*
 * Add a new environment variable
 */
int add_env(t_env **env_list, char *key, char *value) {
  t_env *new_env;
  t_env *current;
  char *new_value;

  if (!env_list || !key || !value)
    return (ERROR);

  // First try to update existing variable
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

  // Create new variable if it doesn't exist
  new_env = create_env_node(key, value);
  if (!new_env)
    return (ERROR);

  // Add to front of list
  new_env->next = *env_list;
  *env_list = new_env;

  return (SUCCESS);
}

/*
 * Update an existing environment variable
 */
int update_env(t_bash *bash, char *key, char *value) {
  int ret;
  char **new_env_array;

  if (!bash || !key || !value)
    return (ERROR);

  ret = add_env(&(bash->env_list), key, value);
  if (ret == SUCCESS) {
    g_env_list = bash->env_list; // Update global env list

    // Update environment array
    new_env_array = create_env_array(bash->env_list);
    if (!new_env_array)
      return (ERROR);

    if (bash->env_array)
      free_env_array(bash->env_array);

    bash->env_array = new_env_array;
  }

  return (ret);
}

/*
 * Delete an environment variable
 */
int delete_env(t_env **env_list, char *key) {
  t_env *current;
  t_env *prev;

  if (!env_list || !*env_list || !key)
    return (ERROR);

  current = *env_list;
  prev = NULL;

  // If it's the first node
  if (ft_strcmp(current->key, key) == 0) {
    *env_list = current->next;
    free(current->key);
    free(current->value);
    free(current);
    return (SUCCESS);
  }

  // Search for the node
  while (current && ft_strcmp(current->key, key) != 0) {
    prev = current;
    current = current->next;
  }

  if (!current)
    return (ERROR); // Key not found

  // Remove the node
  prev->next = current->next;
  free(current->key);
  free(current->value);
  free(current);

  return (SUCCESS);
}

/*
 * Convert environment linked list to array for execve
 */
char **create_env_array(t_env *env_list) {
  t_env *current;
  char **env_array;
  int count;
  int i;
  char *tmp;

  if (!env_list)
    return (NULL);

  // Count number of environment variables
  count = 0;
  current = env_list;
  while (current) {
    count++;
    current = current->next;
  }

  // Allocate memory for environment array
  env_array = (char **)malloc(sizeof(char *) * (count + 1));
  if (!env_array)
    return (NULL);

  // Fill environment array
  i = 0;
  current = env_list;
  while (current) {
    // Allocate memory for "key=value" string
    tmp = malloc(ft_strlen(current->key) + ft_strlen(current->value) + 2);
    if (!tmp) {
      // Free previous allocations
      while (--i >= 0)
        free(env_array[i]);
      free(env_array);
      return (NULL);
    }

    // Create "key=value" string
    sprintf(tmp, "%s=%s", current->key, current->value);
    env_array[i++] = tmp;
    current = current->next;
  }

  env_array[i] = NULL;
  return (env_array);
}

/*
 * Free environment array
 */
void free_env_array(char **env_array) {
  int i;

  if (!env_array)
    return;

  i = 0;
  while (env_array[i]) {
    free(env_array[i]);
    i++;
  }

  free(env_array);
}
