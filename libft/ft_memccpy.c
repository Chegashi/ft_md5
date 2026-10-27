/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:17 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:17 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	int				i;
	unsigned char	*dst_bytes;
	unsigned char	stop;
	unsigned char	*src_bytes;

	src_bytes = (unsigned char *)src;
	stop = (unsigned char)c;
	dst_bytes = (unsigned char *)dst;
	i = 0;
	if (n <= 0)
		return (NULL);
	while (i < (int)n && src_bytes[i] != stop)
	{
		ft_memset(dst_bytes, ((unsigned char *)src)[i], 1);
		i++;
		dst_bytes++;
	}
	if (src_bytes[i] == stop)
		ft_memset(dst_bytes, ((unsigned char *)src)[i], 1);
	if (src_bytes[i] == stop)
		return (dst_bytes + 1);
	else
		return (NULL);
}
