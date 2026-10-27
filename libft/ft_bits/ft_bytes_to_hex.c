/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bytes_to_hex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Writes len bytes as lowercase hex to out (needs len * 2 + 1 bytes).
*/
char	*ft_bytes_to_hex(const uint8_t *data, size_t len, char *out)
{
	static const char	hex[] = "0123456789abcdef";
	size_t				i;

	i = 0;
	while (i < len)
	{
		out[i * 2] = hex[data[i] >> 4];
		out[i * 2 + 1] = hex[data[i] & 0xf];
		i++;
	}
	out[i * 2] = '\0';
	return (out);
}
