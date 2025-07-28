
#include "libft.h"

void	*ft_memmove(void *to, const void *from, size_t size)
{
	unsigned char	*src;
	unsigned char	*dst;

	src = (unsigned char *)from;
	dst = (unsigned char *)to;
	if (!from && !to && size)
		return (NULL);
	if (src == dst)
		return (to);
	if (dst < src)
	{
		while (size--)
			*dst++ = *src++;
	}
	else
	{
		dst += size;
		src += size;
		while (size--)
			*--dst = *--src;
	}
	return (to);
}
