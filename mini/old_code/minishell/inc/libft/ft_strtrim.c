/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 23:02:49 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/10 21:15:26 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_match(const char c, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	st;
	size_t	end;
	size_t	to_len;
	char	*new_s;

	if (!s1 || !set)
		return (NULL);
	st = 0;
	end = ft_strlen(s1);
	while (s1[st] && ft_match(s1[st], set))
		st++;
	if (st == end)
		return (ft_strdup(""));
	end--;
	while (end > st && ft_match(s1[end], set))
		end--;
	to_len = end - st + 1;
	new_s = malloc(to_len + 1);
	if (!new_s)
		return (NULL);
	ft_memcpy(new_s, &s1[st], to_len);
	new_s[to_len] = '\0';
	return (new_s);
}
