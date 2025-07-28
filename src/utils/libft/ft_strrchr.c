
#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char	*s;

	s = str;
	while (*s)
		s++;
	if (c == '\0')
		return ((char *)s);
	while (s >= str)
	{
		if (*s == (char)c)
			return ((char *)s);
		s--;
	}
	return (NULL);
}
