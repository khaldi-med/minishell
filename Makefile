# Simple Makefile for minishell

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lreadline

SRCDIR = src
LIBFT_DIR = $(SRCDIR)/utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = $(SRCDIR)/exec/executor.c \
          $(SRCDIR)/exec/executor_single.c \
          $(SRCDIR)/exec/executor_external.c \
          $(SRCDIR)/exec/executor_pipe.c \
          $(SRCDIR)/exec/redirection.c \
          $(SRCDIR)/exec/signal.c \
          $(SRCDIR)/exec/main.c \
          $(SRCDIR)/exec/builtins.c \
          $(SRCDIR)/exec/builtins_basic.c \
          $(SRCDIR)/exec/builtins_export.c \
          $(SRCDIR)/exec/builtins_exit.c \
          $(SRCDIR)/parsing/heredoc.c \
          $(SRCDIR)/parsing/path.c \
          $(SRCDIR)/parsing/path_utils.c \
          $(SRCDIR)/parsing/expand.c \
          $(SRCDIR)/parsing/quotes.c \
          $(SRCDIR)/parsing/free.c \
          $(SRCDIR)/parsing/parse.c \
          $(SRCDIR)/parsing/parse_utils.c \
          $(SRCDIR)/parsing/parse_cmd.c \
          $(SRCDIR)/parsing/env_basic.c \
          $(SRCDIR)/parsing/env_unset.c \
          $(SRCDIR)/parsing/lexer.c \
          $(SRCDIR)/parsing/token_utils.c \
          $(SRCDIR)/utils/error_handling.c \
          $(SRCDIR)/utils/error_utils.c \

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBFT) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
