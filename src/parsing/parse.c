#include "../../include/minishell.h"

static void ft_pars_args(t_cmd *cmd, t_token **tokens, t_shell *shell) {
  int arg_count;
  int i;
  t_token *current;

  arg_count = ft_count_args(*tokens);
  cmd->args = malloc(sizeof(char *) * (arg_count + 1));
  if (!cmd->args)
    return;
  i = 0;
  current = *tokens;
  while (current && current->type != TOKEN_PIPE) {
    if (current->type == TOKEN_WORD) {
      ft_process_word_token(cmd, current, &i, shell);
      current = current->next;
    } else if (current->type >= TOKEN_REDIR_IN &&
               current->type <= TOKEN_HEREDOC)
      ft_process_redir_token(cmd, &current, shell);
    else
      current = current->next;
  }
  cmd->args[i] = NULL;
  *tokens = current;
}

t_cmd *ft_parse_tokens(t_token *tokens, t_shell *shell) {
  t_cmd *cmds;
  t_cmd *current_cmd;
  t_cmd *new_cmd;

  if (!tokens)
    return (NULL);
  
  /* Check for syntax error: pipe at the beginning */
  if (tokens->type == TOKEN_PIPE) {
    ft_print_syntax_error("|");
    shell->exit_status = MS_SYNTAX_ERROR;
    return (NULL);
  }
  
  cmds = ft_creat_cmd();
  current_cmd = cmds;
  while (tokens) {
    ft_pars_args(current_cmd, &tokens, shell);
    if (tokens && tokens->type == TOKEN_PIPE) {
      tokens = tokens->next;
      
      /* Check for syntax error: pipe at the end */
      if (!tokens) {
        ft_print_syntax_error("newline");
        shell->exit_status = MS_SYNTAX_ERROR;
        ft_free_cmds(cmds);
        return (NULL);
      }
      /* Check for syntax error: consecutive pipes */
      if (tokens->type == TOKEN_PIPE) {
        ft_print_syntax_error("|");
        shell->exit_status = MS_SYNTAX_ERROR;
        ft_free_cmds(cmds);
        return (NULL);
      }
      
      new_cmd = ft_creat_cmd();
      current_cmd->next = new_cmd;
      current_cmd = new_cmd;
    }
  }
  return (cmds);
}
