
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*new_s;

	len = ft_strlen(s) + 1;
	new_s = malloc(len);
	if (!new_s)
		return (NULL);
	ft_memcpy(new_s, s, len);
	return (new_s);
}
