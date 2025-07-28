
#include "libft.h"

char	*ft_strnstr(const char *src, const char *target, size_t n)
{
	size_t	n_len;

	n_len = ft_strlen(target);
	if (!n_len)
		return ((char *)src);
	if (!src && n == 0)
		return (NULL);
	while (*src && n-- >= n_len)
	{
		if (!ft_strncmp(src, target, n_len))
			return ((char *)src);
		src++;
	}
	return (NULL);
}
