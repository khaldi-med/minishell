/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 02:46:27 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/30 19:38:11 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	unsigned long long	n;
	int					sing;
	char				*s;

	s = (char *)str;
	n = 0;
	sing = 1;
	while ((*s >= 9 && *s <= 13) || *s == ' ')
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sing *= -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		if (sing == 1 && n > 9223372036854775807)
			return (-1);
		else if (sing == -1 && n > 9223372036854775807)
			return (0);
		n = (n * 10) + (*s - '0');
		s++;
	}
	return (sing * n);
}
