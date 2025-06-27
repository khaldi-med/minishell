#include "../../include/minishell.h"

char ft_check_unclosed_quotes(char *str) {
  int i;
  char quote_char;

  if (!str)
    return (0);
  i = 0;
  quote_char = 0;
  while (str[i]) {
    if (!quote_char && (str[i] == '\'' || str[i] == '"')) {
      quote_char = str[i];
    } else if (quote_char && str[i] == quote_char) {
      quote_char = 0;
    }
    i++;
  }
  return (quote_char);
}

char *ft_handle_quote_continuation(char *initial_input) {
  char *line;
  char *temp;
  char *result;
  char unclosed_quote;

  result = ft_strdup(initial_input);
  if (!result)
    return (NULL);
  while ((unclosed_quote = ft_check_unclosed_quotes(result)) != 0) {
    line = readline("> ");
    if (!line) {
      free(result);
      return (NULL);
    }
    temp = result;
    result = ft_strjoin_with_newline(result, line);
    free(temp);
    free(line);
    if (!result)
      return (NULL);
  }
  return (result);
}

char *ft_handle_quotes(char *str) {
  char *result;
  int i;
  int j;
  char quote_char;

  if (!str)
    return (NULL);
  result = malloc(ft_strlen(str) + 1);
  if (!result)
    return (NULL);
  i = 0;
  j = 0;
  quote_char = 0;
  while (str[i]) {
    if (!quote_char && (str[i] == '\'' || str[i] == '"')) {
      quote_char = str[i];
      i++;
      continue;
    }
    if (quote_char && str[i] == quote_char) {
      quote_char = 0;
      i++;
      continue;
    }
    result[j++] = str[i++];
  }
  result[j] = '\0';
  return (result);
}
