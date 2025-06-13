#include "../inc/minishell.h"

void ft_free_env_list(t_env *env_list) {
  t_env *current;
  t_env *next;

  current = env_list;
  while (current) {
    next = current->next;
    if (current->key)
      free(current->key);
    if (current->value)
      free(current->value);
    free(current);
    current = next;
  }
}
