
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	s_len;
	size_t	d_len;
	size_t	sp_left;
	char	*d;

	if (dstsize == 0)
		return (ft_strlen(src));
	s_len = ft_strlen(src);
	d_len = ft_strlen(dst);
	d = dst + d_len;
	if (dstsize <= d_len)
		return (dstsize + s_len);
	sp_left = dstsize - d_len - 1;
	while (*src && sp_left > 0)
	{
		*d++ = *src++;
		sp_left--;
	}
	*d = '\0';
	return (d_len + s_len);
}
