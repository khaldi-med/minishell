#include "../../include/minishell.h"

int ft_setup_heredoc(char *delimiter) {
  int pipefd[2];
  char *line;
  pid_t pid;

  if (pipe(pipefd) == -1)
    return (-1);
  pid = fork();
  if (pid == 0) {
    close(pipefd[0]);
    while (1) {
      line = readline("> ");
      if (!line || ft_strcmp(line, delimiter) == 0) {
        if (line)
          free(line);
        break;
      }
      write(pipefd[1], line, ft_strlen(line));
      write(pipefd[1], "\n", 1);
      free(line);
    }
    close(pipefd[1]);
    exit(0);
  } else if (pid > 0) {
    close(pipefd[1]);
    waitpid(pid, NULL, 0);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
  }
  return (0);
}

int ft_preprocess_heredocs(t_cmd *cmds) {
  t_cmd *current;
  t_redir *redir;

  current = cmds;
  while (current) {
    redir = current->redirs;
    while (redir) {
      if (redir->type == TOKEN_HEREDOC) {
        if (ft_process_heredoc_input(redir->file) == -1)
          return (-1);
      }
      redir = redir->next;
    }
    current = current->next;
  }
  return (0);
}

int ft_process_heredoc_input(char *delimiter) {
  char *line;

  while (1) {
    line = readline("> ");
    if (!line || ft_strcmp(line, delimiter) == 0) {
      if (line)
        free(line);
      break;
    }
    free(line);
  }
  return (0);
}
