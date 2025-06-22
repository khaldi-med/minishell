/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:38:36 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/18 23:19:22 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_w(const char *str, const char c)
{
	size_t	count;
	int		in_w;

	count = 0;
	in_w = 0;
	while (*str)
	{
		if (*str != c && !in_w)
		{
			in_w = 1;
			count++;
		}
		else if (*str == c)
			in_w = 0;
		str++;
	}
	return (count);
}

static void	ft_free_arr(char **str, size_t n)
{
	while (n--)
		free(str[n]);
	free(str);
}

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static char	*ft_get_word(const char **str, char c)
{
	const char	*start;
	size_t		len;
	char		*word;

	while (**str == c)
		(*str)++;
	start = *str;
	while (**str && **str != c)
		(*str)++;
	len = *str - start;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**new_s;
	size_t	wrd_c;
	size_t	i;

	if (!s)
		return (NULL);
	wrd_c = ft_count_w(s, c);
	new_s = (char **)malloc(sizeof(char *) * (wrd_c + 1));
	if (!new_s)
		return (NULL);
	i = 0;
	while (i < wrd_c)
	{
		new_s[i] = ft_get_word(&s, c);
		if (!new_s[i])
		{
			ft_free_arr(new_s, i);
			return (NULL);
		}
		i++;
	}
	new_s[wrd_c] = NULL;
	return (new_s);
}
