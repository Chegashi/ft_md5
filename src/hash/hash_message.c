/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_message.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 12:00:00 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hash.h"

/*
** Hashes every full block, then the padded tail:
** the last bytes, 0x80, zeros, and the length in bits.
*/
void	hash_message(t_block_hash *hash, const uint8_t *data, size_t len)
{
	uint8_t	tail[MAX_BLOCK * 2];
	size_t	total;
	size_t	tail_len;

	total = len;
	while (len >= hash->block_size)
	{
		hash->transform(hash->state, data);
		data += hash->block_size;
		len -= hash->block_size;
	}
	ft_bzero(tail, sizeof(tail));
	ft_memcpy(tail, data, len);
	tail[len] = 0x80;
	tail_len = hash->block_size;
	if (len + 1 + hash->length_size > hash->block_size)
		tail_len *= 2;
	ft_store_bytes(tail + tail_len - 8, (uint64_t)total * 8, 8,
		hash->big_endian);
	hash->transform(hash->state, tail);
	if (tail_len > hash->block_size)
		hash->transform(hash->state, tail + hash->block_size);
}
