
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
