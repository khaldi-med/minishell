# 🐚 Minishell

A Unix shell implementation built from scratch in C as part of the 42 School curriculum. This project recreates the functionality of bash, providing a command-line interface with parsing, execution, and built-in command support.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Implementation Details](#implementation-details)
- [Testing](#testing)
- [Known Limitations](#known-limitations)
- [Contributing](#contributing)
- [Authors](#authors)

## 🎯 Overview

Minishell is a simplified version of a Unix shell that provides:
- Interactive command-line interface
- Command parsing and execution
- Built-in command implementations
- I/O redirection support
- Pipeline execution
- Environment variable handling
- Signal management

This project demonstrates understanding of:
- Process management and system calls
- Lexical analysis and parsing
- Memory management
- File descriptors and I/O operations
- Signal handling

## ✨ Features

### Core Functionality
- ✅ **Interactive Shell**: Command prompt with readline support
- ✅ **Command Execution**: Execute external programs and built-ins
- ✅ **Pipeline Support**: Chain commands with pipes (`|`)
- ✅ **I/O Redirection**: Input (`<`), output (`>`), and append (`>>`) redirections
- ✅ **Heredoc**: Here-document support (`<<`)
- ✅ **Environment Variables**: Variable expansion and manipulation
- ✅ **Quote Handling**: Single and double quote parsing
- ✅ **Signal Handling**: Ctrl+C, Ctrl+D, and Ctrl+\\ support
- ✅ **Command History**: Navigate through command history

### Built-in Commands
- `echo` - Display text with `-n` option support
- `cd` - Change directory with support for relative/absolute paths
- `pwd` - Print working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display environment variables
- `exit` - Exit shell with optional status code

### Advanced Features
- **Error Handling**: Comprehensive error messages and status codes
- **Memory Management**: Proper cleanup and leak prevention
- **Path Resolution**: Automatic executable lookup in PATH
- **Variable Expansion**: `$VAR` and `$?` (exit status) expansion
- **Quote Processing**: Proper handling of quoted strings and escapes

## 📁 Project Structure

```
minishell/
├── 📁 include/
│   └── minishell.h          # Main header file with all declarations
├── 📁 src/
│   ├── 📁 exec/             # Command execution logic
│   │   ├── main.c           # Main program entry and shell loop
│   │   ├── executor.c       # Main execution coordinator
│   │   ├── executor_single.c # Single command execution
│   │   ├── executor_external.c # External command handling
│   │   ├── executor_pipe.c  # Pipeline execution
│   │   ├── redirection.c    # I/O redirection handling
│   │   ├── signal.c         # Signal management
│   │   ├── builtins.c       # Built-in command dispatcher
│   │   ├── builtins_basic.c # Basic built-ins (pwd, env)
│   │   ├── builtins_export.c # Export command implementation
│   │   ├── builtins_exit.c  # Exit command with validation
│   │   ├── builtins_exit_utils.c # Exit utility functions
│   │   ├── builtin_echo.c   # Echo command implementation
│   │   ├── builtin_cd.c     # Change directory command
│   │   └── builtin_cd_utils.c # CD utility functions
│   ├── 📁 parsing/          # Input parsing and tokenization
│   │   ├── lexer.c          # Tokenization and lexical analysis
│   │   ├── parse.c          # Main parsing logic
│   │   ├── parse_cmd.c      # Command structure creation
│   │   ├── parse_utils.c    # Parsing utility functions
│   │   ├── token_utils.c    # Token manipulation functions
│   │   ├── quotes.c         # Quote handling and processing
│   │   ├── expand.c         # Variable expansion logic
│   │   ├── expand_2.c       # Additional expansion utilities
│   │   ├── heredoc.c        # Here-document implementation
│   │   ├── heredoc_2.c      # Additional heredoc utilities
│   │   ├── path.c           # Path resolution and validation
│   │   ├── path_utils.c     # Path utility functions
│   │   ├── env_basic.c      # Environment variable handling
│   │   ├── env_unset.c      # Environment variable removal
│   │   └── free.c           # Memory cleanup functions
│   └── 📁 utils/            # Utility functions and libraries
│       ├── 📁 libft/        # Custom C library implementation
│       ├── error_handling.c # Error message and handling
│       └── error_handeling_2.c # Additional error utilities
├── Makefile                 # Build configuration
└── README.md               # This file
```

## 🔧 Installation

### Prerequisites
- GCC compiler
- GNU Readline library
- Make

# Clone the repository
git clone [your-repository-url]
cd minishell

# Build the project
make

### Build Commands
```bash
make        # Build the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Rebuild everything
```

## 🚀 Usage

### Starting the Shell
```bash
./minishell
```

### Basic Examples
```bash
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell
-rwxr-xr-x  1 user  staff  50000 Jan 15 10:30 minishell

minishell$ export MY_VAR="test"
minishell$ echo $MY_VAR
test

minishell$ cd /tmp && pwd
/tmp

minishell$ cat < input.txt > output.txt

minishell$ cat << EOF
> This is a heredoc
> Multiple lines supported
> EOF
This is a heredoc
Multiple lines supported
```

### Advanced Usage
```bash
# Pipeline with multiple commands
minishell$ ps aux | grep bash | wc -l

# Complex redirection
minishell$ echo "Error message" >&2

# Environment manipulation
minishell$ export PATH="/usr/local/bin:$PATH"
minishell$ unset MY_VAR

# Exit with status
minishell$ exit 42
```

## 🔍 Implementation Details

### Architecture
The shell follows a modular architecture with clear separation of concerns:

1. **Lexical Analysis**: Input string → Token stream
2. **Parsing**: Token stream → Command structure
3. **Expansion**: Variable and quote processing
4. **Execution**: Command execution with proper I/O handling


### Edge Cases to Test
- Empty input
- Multiple spaces and tabs
- Unclosed quotes
- Invalid commands
- Permission errors
- Signal handling (Ctrl+C, Ctrl+D)
- Complex pipelines
- Nested quotes
- Variable expansion edge cases

## 👥 Authors

- **Mohammed** - [@mohkhald](https://github.com/mohkhald) 
- **Collaborator** - [@ChimexXy](https://github.com/ChimexXy)

---

**Note**: This implementation is for educational purposes as part of the 42 School curriculum. It demonstrates understanding of systems programming, process management, and Unix shell concepts.
