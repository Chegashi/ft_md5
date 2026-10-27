/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bytes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Reads size bytes (1 to 8) from in as one integer, in the given byte order.
*/
uint64_t	ft_load_bytes(const uint8_t *in, int size, int big_endian)
{
	uint64_t	value;
	int			i;

	value = 0;
	i = -1;
	while (++i < size)
	{
		if (big_endian)
			value = value << 8 | in[i];
		else
			value |= (uint64_t)in[i] << (8 * i);
	}
	return (value);
}

/*
** Writes the low size bytes (1 to 8) of value to out, in the given byte order.
*/
void	ft_store_bytes(uint8_t *out, uint64_t value, int size, int big_endian)
{
	int	i;

	i = -1;
	while (++i < size)
	{
		if (big_endian)
			out[size - 1 - i] = (uint8_t)(value >> (8 * i));
		else
			out[i] = (uint8_t)(value >> (8 * i));
	}
}
