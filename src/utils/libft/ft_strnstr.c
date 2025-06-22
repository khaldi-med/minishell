/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:50:24 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/20 08:32:05 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *src, const char *target, size_t n)
{
	size_t	n_len;

	n_len = ft_strlen(target);
	if (!n_len)
		return ((char *)src);
	if (!src && n == 0)
		return (NULL);
	while (*src && n-- >= n_len)
	{
		if (!ft_strncmp(src, target, n_len))
			return ((char *)src);
		src++;
	}
	return (NULL);
}
