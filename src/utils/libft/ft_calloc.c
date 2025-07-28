
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*array;
	size_t	t_s;

	if (size != 0 && count > 18446744073709551615UL / size)
		return (NULL);
	t_s = count * size;
	array = malloc(t_s);
	if (!array)
		return (NULL);
	ft_memset(array, 0, t_s);
	return (array);
}
