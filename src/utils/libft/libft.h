/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 23:44:52 by mohkhald          #+#    #+#             */
/*   Updated: 2025/06/23 15:36:56 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

// Builte in library
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
// My libft functions

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// Part I
void				ft_bzero(void *s, size_t n);
void				*ft_memset(void *str, int c, size_t z);
void				*ft_memchr(const void *block, int c, size_t size);
void				*ft_memcpy(void *to, const void *from, size_t n);
void				*ft_memmove(void *to, const void *from, size_t size);
void				*ft_calloc(size_t count, size_t size);
int					ft_memcmp(const void *a1, const void *a2, size_t size);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_atoi(const char *str);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strdup(const char *s);
char				*ft_strrchr(const char *str, int c);
char				*ft_strchr(const char *str, int c);
char				*ft_strnstr(const char *src, const char *target, size_t n);
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dst, const char *src, size_t size);

// Part II
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

// Libft Bonus Part
t_list				*ft_lstnew(void *content);

t_list				*ft_lstlast(t_list *lst);

void				ft_lstadd_front(t_list **lst, t_list *new);

void				ft_lstadd_back(t_list **lst, t_list *new);

int					ft_lstsize(t_list *lst);

#endif
