
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
