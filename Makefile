# Minishell Makefile
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Libraries
LIBS = -lreadline

# Include directories
INC = -I$(INC_DIR)

# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $(NAME)
	@echo "Minishell compiled successfully"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files removed"

fclean: clean
	rm -f $(NAME)
	@echo "Executable removed"

re: fclean all

.PHONY: all clean fclean re

