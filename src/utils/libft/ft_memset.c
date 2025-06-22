/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 02:15:00 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/25 10:28:09 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *block, int c, size_t size)
{
	unsigned char	*str;

	str = block;
	while (size--)
	{
		*str++ = (unsigned char)c;
	}
	return (block);
}
