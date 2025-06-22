/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohkhald <mohkhald@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:52:36 by mohkhald          #+#    #+#             */
/*   Updated: 2024/11/23 10:07:27 by mohkhald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(unsigned int n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	ft_fill_str(char *s, unsigned int n, int len, int sgin)
{
	int	i;

	i = len - 1;
	if (n == 0)
		s[0] = '0';
	while (n > 0)
	{
		s[i--] = (n % 10) + '0';
		n /= 10;
	}
	if (sgin == -1)
		s[0] = '-';
}

char	*ft_itoa(int n)
{
	int				sign;
	char			*str;
	unsigned int	num;
	int				len;

	sign = 1;
	num = 0;
	len = 0;
	if (n < 0)
	{
		sign *= -1;
		num = n * -1;
		len = ft_len(num) + 1;
	}
	else
	{
		num = n;
		len = ft_len(num);
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	ft_fill_str(str, num, len, sign);
	return (str);
}
