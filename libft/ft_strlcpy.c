/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:16 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:16 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	j;
	size_t	s_s;

	j = 0;
	if (!dst)
		return (0);
	s_s = ft_strlen((char *)src);
	while (src[j] != '\0' && j + 1 < size)
	{
		dst[j] = src[j];
		j++;
	}
	if (size != 0)
		dst[j] = '\0';
	return (s_s);
}
