/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rotate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uint32_t	ft_rotl32(uint32_t value, int bits)
{
	return ((value << bits) | (value >> (32 - bits)));
}

uint32_t	ft_rotr32(uint32_t value, int bits)
{
	return ((value >> bits) | (value << (32 - bits)));
}

uint64_t	ft_rotl64(uint64_t value, int bits)
{
	return ((value << bits) | (value >> (64 - bits)));
}

uint64_t	ft_rotr64(uint64_t value, int bits)
{
	return ((value >> bits) | (value << (64 - bits)));
}
