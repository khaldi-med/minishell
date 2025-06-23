# MINISHELL FUNCTION CALL TREE

```
main() [main.c]
├── ft_init_shell()
│   └── ft_copy_env() [env_basic.c]
│       └── ft_strdup() [libft]
├── ft_shell_loop()
│   ├── ft_setup_signals() [signal.c]
│   ├── readline() [external library]
│   ├── add_history() [external library]  
│   ├── ft_tokenize() [lexer.c]
│   │   ├── ft_skip_space() [token_utils.c]
│   │   ├── ft_handle_pipe() [lexer.c]
│   │   │   └── ft_creat_token() [token_utils.c]
│   │   │       └── ft_strdup() [libft]
│   │   ├── ft_handle_redirection() [token_utils.c]
│   │   │   ├── ft_get_redir_type() [token_utils.c]
│   │   │   └── ft_creat_token() [token_utils.c]
│   │   ├── ft_handle_word() [lexer.c]
│   │   │   ├── ft_extract_word() [lexer.c]
│   │   │   │   └── ft_strlcpy() [libft]
│   │   │   └── ft_creat_token() [token_utils.c]
│   │   └── ft_add_token() [token_utils.c]
│   ├── ft_parse_tokens() [parse.c]
│   │   ├── ft_creat_cmd() [parse_cmd.c]
│   │   └── ft_pars_args() [parse.c]
│   │       ├── ft_count_args() [parse_utils.c]
│   │       ├── ft_process_word_token() [parse_utils.c]
│   │       │   ├── ft_expand_variables() [expand.c]
│   │       │   │   ├── ft_in_singlea_q() [expand.c]
│   │       │   │   ├── ft_expand_dollar() [expand.c]
│   │       │   │   │   ├── ft_extract_var_name() [expand.c]
│   │       │   │   │   │   ├── ft_isalpha() [libft]
│   │       │   │   │   │   ├── ft_isalnum() [libft]
│   │       │   │   │   │   └── ft_strlcpy() [libft]
│   │       │   │   │   └── ft_get_variable_value() [expand.c]
│   │       │   │   │       ├── ft_strcmp() [libft]
│   │       │   │   │       ├── ft_itoa() [libft]
│   │       │   │   │       ├── ft_get_env_value() [env_basic.c]
│   │       │   │   │       │   ├── ft_strlen() [libft]
│   │       │   │   │       │   ├── ft_strncmp() [libft]
│   │       │   │   │       │   └── ft_strdup() [libft]
│   │       │   │   │       └── ft_strdup() [libft]
│   │       │   │   ├── ft_strjoin() [libft]
│   │       │   │   └── ft_strlen() [libft]
│   │       │   └── ft_handle_quotes() [quotes.c]
│   │       │       └── ft_strlen() [libft]
│   │       └── ft_process_redir_token() [parse_utils.c]
│   │           ├── ft_expand_variables() [expand.c] (same as above)
│   │           ├── ft_handle_quotes() [quotes.c] (same as above)
│   │           └── ft_add_redir() [parse_cmd.c]
│   │               └── ft_strdup() [libft]
│   ├── ft_exec_cmds() [executor.c]
│   │   ├── ft_preprocess_heredocs() [heredoc.c]
│   │   │   └── ft_process_heredoc_input() [heredoc.c]
│   │   │       ├── readline() [external library]
│   │   │       └── ft_strcmp() [libft]
│   │   ├── ft_exec_sing_cmd() [executor_single.c]
│   │   │   ├── ft_is_builtin() [builtins.c]
│   │   │   │   └── ft_strcmp() [libft]
│   │   │   ├── ft_exec_built_with_redir() [executor_single.c]
│   │   │   │   ├── ft_save_fds() [executor_single.c]
│   │   │   │   │   └── dup() [system call]
│   │   │   │   ├── ft_setup_redirections() [redirection.c]
│   │   │   │   ├── ft_exec_builtin() [builtins.c]
│   │   │   │   │   ├── ft_builtin_echo() [builtins_basic.c]
│   │   │   │   │   ├── ft_builtin_cd() [builtins_basic.c]
│   │   │   │   │   ├── ft_builtin_pwd() [builtins_basic.c]
│   │   │   │   │   ├── ft_builtin_export() [builtins_export.c]
│   │   │   │   │   ├── ft_builtin_unset() [env_unset.c]
│   │   │   │   │   ├── ft_builtin_env() [builtins_basic.c]
│   │   │   │   │   └── ft_builtin_exit() [builtins_exit.c]
│   │   │   │   └── ft_restore_fds() [executor_single.c]
│   │   │   │       ├── dup2() [system call]
│   │   │   │       └── close() [system call]
│   │   │   ├── ft_exec_builtin() [builtins.c] (same as above)
│   │   │   ├── fork() [system call]
│   │   │   ├── ft_exec_external_cmd() [executor_external.c]
│   │   │   │   ├── ft_setup_redirections() [redirection.c]
│   │   │   │   ├── ft_find_executable() [path.c]
│   │   │   │   │   ├── ft_get_env_value() [env_basic.c] (same as above)
│   │   │   │   │   ├── ft_split() [libft]
│   │   │   │   │   ├── ft_strjoin() [libft]
│   │   │   │   │   └── access() [system call]
│   │   │   │   └── execve() [system call]
│   │   │   └── ft_wait_for_child() [executor_single.c]
│   │   │       └── waitpid() [system call]
│   │   └── ft_exec_pipe() [executor_pipe.c]
│   │       ├── ft_creat_pip_poces() [executor_pipe.c]
│   │       │   ├── pipe() [system call]
│   │       │   ├── fork() [system call]
│   │       │   ├── ft_setup_pip_redi() [executor_pipe.c]
│   │       │   │   ├── dup2() [system call]
│   │       │   │   └── close() [system call]
│   │       │   └── ft_exec_cmd_in_child() [executor_pipe.c]
│   │       │       ├── ft_setup_redirections() [redirection.c]
│   │       │       ├── ft_is_builtin() [builtins.c] (same as above)
│   │       │       ├── ft_exec_builtin() [builtins.c] (same as above)
│   │       │       ├── ft_find_executable() [path.c] (same as above)
│   │       │       └── execve() [system call]
│   │       ├── ft_clean_pip_fds() [executor_pipe.c]
│   │       │   └── close() [system call]
│   │       ├── wait() [system call]
│   │       └── waitpid() [system call]
│   ├── ft_free_tokens() [free.c]
│   ├── ft_free_cmds() [free.c]
│   └── free() [system call]
├── ft_cleanup_shell() [free.c]
└── rl_clear_history() [external library]
```

## FILE ORGANIZATION BY MODULES:

### **MAIN ENTRY POINT**
- `main.c`: Program entry point and shell loop

### **LEXICAL ANALYSIS (TOKENIZATION)**
- `lexer.c`: Main tokenization logic
- `token_utils.c`: Token creation and manipulation utilities

### **PARSING**
- `parse.c`: Main parsing logic (2 functions)
- `parse_utils.c`: Token processing utilities (3 functions) 
- `parse_cmd.c`: Command and redirection creation (2 functions)
- `expand.c`: Variable expansion
- `quotes.c`: Quote handling

### **EXECUTION**
- `executor.c`: Main execution dispatcher
- `executor_single.c`: Single command execution
- `executor_pipe.c`: Pipeline execution  
- `executor_external.c`: External command execution
- `redirection.c`: I/O redirection handling
- `heredoc.c`: Here-document processing

### **BUILT-IN COMMANDS**
- `builtins.c`: Built-in command dispatcher
- `builtins_basic.c`: Basic built-ins (echo, cd, pwd, env)
- `builtins_export.c`: Export built-in
- `builtins_exit.c`: Exit built-in

### **ENVIRONMENT MANAGEMENT**
- `env_basic.c`: Basic environment operations
- `env_unset.c`: Unset built-in

### **PATH RESOLUTION**
- `path.c`: Executable path finding
- `path_utils.c`: Path utilities

### **SIGNAL HANDLING**
- `signal.c`: Signal setup and handling

### **MEMORY MANAGEMENT**
- `free.c`: Memory cleanup functions

### **UTILITIES**
- `libft/`: Custom C library functions

## FUNCTION COUNT PER FILE:
- `parse.c`: 2 functions ✅
- `parse_utils.c`: 3 functions ✅  
- `parse_cmd.c`: 2 functions ✅
- All other files: ≤ 5 functions each ✅

## KEY EXECUTION FLOWS:

1. **Input Processing**: `main()` → `ft_shell_loop()` → `ft_tokenize()` → `ft_parse_tokens()`

2. **Single Command**: `ft_exec_cmds()` → `ft_exec_sing_cmd()` → `ft_exec_builtin()` or `ft_exec_external_cmd()`

3. **Pipeline**: `ft_exec_cmds()` → `ft_exec_pipe()` → `ft_creat_pip_poces()` → `ft_exec_cmd_in_child()`

4. **Variable Expansion**: `ft_process_word_token()` → `ft_expand_variables()` → `ft_expand_dollar()` → `ft_get_variable_value()`

5. **Path Resolution**: `ft_exec_external_cmd()` → `ft_find_executable()` → `ft_get_env_value()`
