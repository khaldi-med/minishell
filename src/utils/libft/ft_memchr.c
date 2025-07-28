
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
