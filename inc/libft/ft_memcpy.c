/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 02:49:55 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/30 19:32:25 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *to, const void *from, size_t n)
{
	unsigned char	*src;
	unsigned char	*dst;

	if (to == from)
		return (to);
	src = (unsigned char *)from;
	dst = (unsigned char *)to;
	while (n--)
		*dst++ = *src++;
	return (to);
}
