
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
