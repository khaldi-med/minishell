# Minishell project Makefile

NAME = minishell

# Compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Libft library
LIBFT_DIR = inc/libft
LIBFT = $(LIBFT_DIR)/libft.a

# Header files
INC_DIR = inc
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)

# Source directories
PARS_DIR = parsing/pars
UTILS_DIR = parsing/utils
EXC_DIR = exc

# Source files
PARS_SRC = $(PARS_DIR)/parsing.c \
           $(PARS_DIR)/parsing1.c \
           $(PARS_DIR)/parsing2.c \
           $(PARS_DIR)/tokenizer.c \
           $(PARS_DIR)/redirection.c \
           $(PARS_DIR)/envirement.c

UTILS_SRC = $(UTILS_DIR)/ft_split.c \
            $(UTILS_DIR)/ft_strdup.c \
            $(UTILS_DIR)/ft_free.c \
            $(UTILS_DIR)/free_utils.c

EXC_SRC = $(EXC_DIR)/builtin.c \
          $(EXC_DIR)/builtin2.c \
          $(EXC_DIR)/env_utils.c \
          $(EXC_DIR)/exec.c \
          $(EXC_DIR)/pipeline.c \
          $(EXC_DIR)/redirection.c \
          $(EXC_DIR)/signal.c

MAIN_SRC = main.c

# All source files
SRC = $(PARS_SRC) $(UTILS_SRC) $(EXC_SRC) $(MAIN_SRC)

# Object files
OBJ_DIR = obj
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

# Create necessary directories
DIRS = $(OBJ_DIR) \
       $(OBJ_DIR)/$(PARS_DIR) \
       $(OBJ_DIR)/$(UTILS_DIR) \
       $(OBJ_DIR)/$(EXC_DIR)

# Default rule
all: $(NAME)

# Compile libft
$(LIBFT):
	@echo "Compiling libft..."
	@$(MAKE) -C $(LIBFT_DIR)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(DIRS)

# Compile source files to object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link object files to create the executable
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -L$(LIBFT_DIR) -lft
	@echo "$(NAME) compiled successfully!"

# Clean object files
clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "Object files cleaned!"

# Clean object files and executable
fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(NAME) executable removed!"

# Clean and rebuild
re: fclean all

# Phony targets
.PHONY: all clean fclean re

