#include "libft.h"

static int	calc_num_len(int num, int *is_negative)
{
	int	len;
	int	temp;

	*is_negative = 0;
	if (num < 0)
	{
		*is_negative = 1;
		num = -num;
	}
	temp = num;
	len = 0;
	if (temp == 0)
		len = 1;
	else
	{
		while (temp > 0)
		{
			temp /= 10;
			len++;
		}
	}
	if (*is_negative)
		len++;
	return (len);
}

static void	fill_num_buffer(char *str, int num, int len, int is_negative)
{
	int	index;

	str[len] = '\0';
	index = len - 1;
	if (num == 0)
		str[0] = '0';
	else
	{
		while (num > 0)
		{
			str[index] = (num % 10) + '0';
			num /= 10;
			index--;
		}
	}
	if (is_negative)
		str[0] = '-';
}

char	*ft_itoa(int num)
{
	char	*str;
	int		len;
	int		is_negative;

	if (num == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = calc_num_len(num, &is_negative);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	fill_num_buffer(str, num < 0 ? -num : num, len, is_negative);
	return (str);
}
