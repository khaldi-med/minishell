
NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I.

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/minishell.c \
	   $(SRC_DIR)/signals.c \
	   $(SRC_DIR)/env_init.c \
	   $(SRC_DIR)/env_utils.c \
	   $(SRC_DIR)/shell_utils.c \
	   $(SRC_DIR)/lexer.c \
	   $(SRC_DIR)/parser.c \
	   $(SRC_DIR)/executor.c \
	   $(SRC_DIR)/builtins.c \
	   $(SRC_DIR)/builtins2.c \
	   $(SRC_DIR)/utils.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

READLINE_FLAGS = -lreadline -L/usr/local/lib -I/usr/local/include

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(READLINE_FLAGS)
	@echo "Minishell compiled successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed."

re: fclean all

.PHONY: all clean fclean re
