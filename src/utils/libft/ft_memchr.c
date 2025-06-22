/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:40:39 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/24 05:36:21 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *block, int c, size_t size)
{
	unsigned char	*src;

	src = (unsigned char *)block;
	while (size--)
	{
		if (*src == (unsigned char)c)
			return ((void *)src);
		src++;
	}
	return (NULL);
}
