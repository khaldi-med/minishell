
#include "../../include/minishell.h"

void ft_process_word_token(t_cmd *cmd, t_token *token, int *index,
                           t_shell *shell) {
  char *expanded_value;
  char *processed_value;

  expanded_value = ft_expand_variables(token->value, shell, token->type);
  processed_value = ft_handle_quotes(expanded_value);
  cmd->args[*index] = processed_value;
  free(expanded_value);
  (*index)++;
}

int ft_count_args(t_token *tokens) {
  int count;

  count = 0;
  while (tokens && tokens->type != TOKEN_PIPE) {
    if (tokens->type == TOKEN_WORD)
      count++;
    tokens = tokens->next;
  }
  return (count);
}

static void ft_handle_redir_file(t_cmd *cmd, t_token **token, t_shell *shell,
                                 t_token_type redir_type) {
  char *expanded_value;
  char *processed_value;

  expanded_value = ft_expand_variables((*token)->value, shell, redir_type);
  processed_value = ft_handle_quotes(expanded_value);
  ft_add_redir(cmd, redir_type, processed_value);
  free(expanded_value);
  free(processed_value);
  *token = (*token)->next;
}

void ft_process_redir_token(t_cmd *cmd, t_token **token, t_shell *shell) {
  t_token_type redir_type;

  redir_type = (*token)->type;
  *token = (*token)->next;
  if (*token && (*token)->type == TOKEN_WORD) {
    ft_handle_redir_file(cmd, token, shell, redir_type);
  } else {
    if (shell->exit_status != SYNTAX_ERROR) {
      if (!*token)
        ft_print_syntax_error("newline");
      else
        ft_print_syntax_error((*token)->value);
      shell->exit_status = SYNTAX_ERROR;
    }
  }
}

char *ft_strjoin_with_newline(char *s1, char *s2) {
  char *result;
  size_t len1;
  size_t len2;
  size_t i;

  if (!s1 || !s2)
    return (NULL);
  len1 = ft_strlen(s1);
  len2 = ft_strlen(s2);
  result = malloc(len1 + len2 + 2);
  if (!result)
    return (NULL);
  i = -1;
  while (++i < len1)
    result[i] = s1[i];
  result[i++] = '\n';
  while (i - len1 - 1 < len2) {
    result[i] = s2[i - len1 - 1];
    i++;
  }
  result[i] = '\0';
  return (result);
}
