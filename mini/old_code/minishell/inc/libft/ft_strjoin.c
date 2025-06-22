/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 00:33:29 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/08 04:08:23 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*new;
	char	*n_p;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new = malloc(len_s1 + len_s2 + 1);
	if (!new)
		return (NULL);
	n_p = new;
	while (*s1)
		*new ++ = *s1++;
	while (*s2)
		*new ++ = *s2++;
	*new = '\0';
	return (n_p);
}
