#include "./inc/minishell.h"

/*
 * Global variable for exit status
 */
int g_exit_status = 0;

/*
 * Update global exit status
 */
void update_exit_status(int status) { g_exit_status = status; }

/*
 * Initialize shell: set up environment and signals
 */
int initialize_shell(t_bash *bash, char **envp) {
  if (!bash || !envp)
    return (ERROR);
  // Initialize all fields to NULL/0 first
  bash->env_list = NULL;
  bash->env_array = NULL;
  bash->s_cmd = NULL;
  bash->commands = NULL;
  bash->args_pip = NULL;
  bash->num_cmd = 0;
  // Initialize environment variables
  if (init_env(bash, envp) == ERROR) {
    fprintf(stderr, "minishell: failed to initialize environment\n");
    return (ERROR);
  }
  // Convert environment linked list to array for execve
  bash->env_array = create_env_array(bash->env_list);
  if (!bash->env_array) {
    fprintf(stderr, "minishell: failed to create environment array\n");
    free_env_list(bash->env_list);
    bash->env_list = NULL;
    return (ERROR);
  }
  return (SUCCESS);
}

/*
 * Clean up after command execution
 */
void cleanup_command(t_bash *bash) {
  int i;

  // Free command structures
  if (bash->s_cmd) {
    i = 0;
    while (i < bash->num_cmd) {
      if (bash->s_cmd[i]) {
        if (bash->s_cmd[i]->command)
          free(bash->s_cmd[i]->command);
        if (bash->s_cmd[i]->arguments)
          free_array(bash->s_cmd[i]->arguments);
        if (bash->s_cmd[i]->s_red)
          free_redirection(bash->s_cmd[i]->s_red);
        free(bash->s_cmd[i]);
      }
      i++;
    }
    free(bash->s_cmd);
    bash->s_cmd = NULL;
  }
  // Free other command-related memory
  if (bash->commands) {
    free(bash->commands);
    bash->commands = NULL;
  }
  if (bash->args_pip) {
    free_array(bash->args_pip);
    bash->args_pip = NULL;
  }
}

/*
 * Main shell loop
 */
void shell_loop(t_bash *bash) {
  char *input;

  while (1) {
    // Display prompt and read input
    input = readline("minishell$ ");
    // Handle EOF (Ctrl+D)
    if (!input) {
      printf("exit\n");
      break;
    }
    // Skip empty input
    if (!*input) {
      free(input);
      continue;
    }
    // Add to history
    add_history(input);
    // Parse input
    if (parse_input(bash, input) != SUCCESS) {
      free(input);
      continue;
    }
    // Commands are executed within parse_input now
    free(input);
  }
}

/*
 * Execute a single command chain separated by semicolons
 */
int execute_command_chain(t_bash *bash, char *command_chain) {
  int exit_status;

  exit_status = 0;
  // Set command string
  bash->commands = ft_strdup(command_chain);
  if (!bash->commands)
    return (ERROR);
  // Parse using the parsing functions
  if (!check_cmd(bash->commands) || !check_cmd1(bash->commands)) {
    fprintf(stderr, "minishell: syntax error\n");
    free(bash->commands);
    bash->commands = NULL;
    return (ERROR);
  }
  // Count number of commands (separated by pipes)
  bash->num_cmd = count_pipes(bash->commands);
  // Allocate memory for command structures
  bash->s_cmd = malloc(sizeof(t_cmd *) * (bash->num_cmd + 1));
  if (!bash->s_cmd) {
    free(bash->commands);
    bash->commands = NULL;
    return (ERROR);
  }
  // Split input by pipes
  bash->args_pip = ft_split(bash->commands, '|');
  if (!bash->args_pip) {
    free(bash->s_cmd);
    bash->s_cmd = NULL;
    free(bash->commands);
    bash->commands = NULL;
    return (ERROR);
  }
  // Process each command
  if (!select_struct2(bash)) {
    cleanup_command(bash);
    return (ERROR);
  }
  // Tokenize commands and arguments
  if (!select_struct3(bash)) {
    cleanup_command(bash);
    return (ERROR);
  }
  // Check for redirections and environment variables
  check_red_env(bash);
  // Execute commands
  exit_status = execute_commands(bash);
  return (exit_status);
}

/*
 * Parse input string and handle semicolon-separated commands
 */
int execute_logical_chain(t_bash *bash, char *input) {
  int left_result;
  int is_and_operator;
  char *trimmed_input;
  char *trimmed_right;
  int right_result;

  char *and_pos, *or_pos, *split_pos;
  char *left_cmd, *right_cmd;
  is_and_operator = 0;
  // Trim whitespace from input
  trimmed_input = input;
  while (*trimmed_input && (*trimmed_input == ' ' || *trimmed_input == '\t'))
    trimmed_input++;
  // Check for empty input
  if (!*trimmed_input)
    return (0);
  // Find && and || operators (not in quotes)
  and_pos = strstr(trimmed_input, "&&");
  or_pos = strstr(trimmed_input, "||");
  // Determine which operator comes first
  if (and_pos && or_pos) {
    if (and_pos < or_pos) {
      split_pos = and_pos;
      is_and_operator = 1;
    } else {
      split_pos = or_pos;
      is_and_operator = 0;
    }
  } else if (and_pos) {
    split_pos = and_pos;
    is_and_operator = 1;
  } else if (or_pos) {
    split_pos = or_pos;
    is_and_operator = 0;
  } else {
    // No logical operators found, execute as single command
    return (execute_command_chain(bash, trimmed_input));
  }
  // Check if we have a command before the operator
  if (split_pos == trimmed_input) {
    // Operator at the beginning
    fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
            is_and_operator ? "&&" : "||");
    return (ERROR);
  }
  // Split into left and right parts
  left_cmd = malloc(split_pos - trimmed_input + 1);
  if (!left_cmd)
    return (ERROR);
  strncpy(left_cmd, trimmed_input, split_pos - trimmed_input);
  left_cmd[split_pos - trimmed_input] = '\0';
  right_cmd = ft_strdup(split_pos + 2); // Skip past && or ||
  if (!right_cmd) {
    free(left_cmd);
    return (ERROR);
  }
  // Trim the right command
  trimmed_right = right_cmd;
  while (*trimmed_right && (*trimmed_right == ' ' || *trimmed_right == '\t'))
    trimmed_right++;
  // Check if we have a command after the operator
  if (!*trimmed_right) {
    fprintf(stderr,
            "minishell: syntax error near unexpected token `newline'\n");
    free(left_cmd);
    free(right_cmd);
    return (ERROR);
  }
  // Execute left command
  left_result = execute_logical_chain(bash, left_cmd);
  cleanup_command(bash);
  // Execute right command based on operator and left result
  if ((is_and_operator && left_result == 0) ||
      (!is_and_operator && left_result != 0)) {
    free(left_cmd);
    right_result = execute_logical_chain(bash, right_cmd);
    free(right_cmd);
    return (right_result);
  }
  free(left_cmd);
  free(right_cmd);
  return (left_result);
}

int parse_input(t_bash *bash, char *input) {
  char **command_chains;
  int i;
  int last_exit_status;

  last_exit_status = 0;
  // Split input by semicolons
  command_chains = ft_split(input, ';');
  if (!command_chains)
    return (ERROR);
  // Execute each command chain
  i = 0;
  while (command_chains[i]) {
    // Skip empty command chains
    if (*command_chains[i]) {
      last_exit_status = execute_logical_chain(bash, command_chains[i]);
      g_exit_status = last_exit_status;
      // Clean up after each command chain
      cleanup_command(bash);
    }
    free(command_chains[i]);
    i++;
  }
  free(command_chains);
  return (SUCCESS);
}

/*
 * Clean up resources before exiting
 */
void cleanup_shell(t_bash *bash) {
  // Clean up any remaining command data
  cleanup_command(bash);
  // Free environment variables
  free_env_array(bash->env_array);
  free_env_list(bash->env_list);
  // Free bash structure
  free(bash);
}

/*
 * Signal handler setup
 */
void setup_signals(void) {
  // Use the consolidated signal setup function
  active_signal();
}

/*
 * Main function
 */
int main(int argc, char **argv, char **envp) {
  t_bash *bash;

  (void)argc;
  (void)argv;
  // Allocate memory for shell structure
  bash = malloc(sizeof(t_bash));
  if (!bash)
    return (EXIT_FAILURE);
  // Initialize shell
  if (initialize_shell(bash, envp) == ERROR) {
    free(bash);
    return (EXIT_FAILURE);
  }
  // Set up signal handlers
  setup_signals();
  // Run main shell loop
  shell_loop(bash);
  // Clean up and exit
  cleanup_shell(bash);
  return (g_exit_status);
}
