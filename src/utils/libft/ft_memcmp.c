/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:47:27 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/24 05:33:48 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *a1, const void *a2, size_t size)
{
	unsigned char	*m1;
	unsigned char	*m2;

	m1 = (unsigned char *)a1;
	m2 = (unsigned char *)a2;
	while (size--)
	{
		if (*m1 != *m2)
			return (*m1 - *m2);
		m2++;
		m1++;
	}
	return (0);
}
