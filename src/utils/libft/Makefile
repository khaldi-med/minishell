# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 07:11:28 by mohkhald          #+#    #+#              #
#    Updated: 2024/11/30 19:31:59 by mohkhald         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRCS =  ft_putendl_fd.c ft_isalpha.c ft_isdigit.c ft_atoi.c\
        ft_putstr_fd.c ft_calloc.c ft_isalnum.c ft_isascii.c\
        ft_strmapi.c ft_split.c ft_isprint.c ft_strlen.c\
        ft_striteri.c ft_memset.c ft_bzero.c ft_memchr.c ft_memcpy.c\
        ft_memcmp.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
        ft_putchar_fd.c ft_strtrim.c ft_toupper.c ft_tolower.c \
        ft_itoa.c ft_substr.c ft_strchr.c ft_strncmp.c \
        ft_strnstr.c ft_strrchr.c ft_strjoin.c ft_strdup.c\
        ft_putnbr_fd.c ft_strcmp.c

BSRCS =  ft_lstnew_bonus.c ft_lstadd_front_bonus.c\
	     ft_lstsize_bonus.c ft_lstlast_bonus.c\
	     ft_lstadd_back_bonus.c


NAME = libft.a


OBJ = $(SRCS:.c=.o)


BOBJ = $(BSRCS:.c=.o)


CC = cc


FLAGS = -Wall -Wextra -Werror


all: $(NAME)


%.o: %.c libft.h
	@$(CC) $(FLAGS) -c $< -o $@


$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)


bonus: $(BOBJ)
	@ar rcs $(NAME) $(BOBJ)


clean:
	@rm -f $(OBJ) $(BOBJ)


fclean: clean
	@rm -f $(NAME)


re: fclean all


.PHONY: all bonus clean fclean re
