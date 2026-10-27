/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 12:00:00 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:25:09 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hash.h"

/*
** 0x0b... values have a leading zero only to get past a norminette bug.
*/
static const uint32_t	g_sha256_k[64] = {
	0x428a2f98, 0x71374491, 0x0b5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0x0b00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t	big_sigma(uint32_t x, int r1, int r2, int r3)
{
	return (ft_rotr32(x, r1) ^ ft_rotr32(x, r2)
		^ ft_rotr32(x, r3));
}

static uint32_t	small_sigma(uint32_t x, int r1, int r2, int shift)
{
	return (ft_rotr32(x, r1) ^ ft_rotr32(x, r2) ^ (x >> shift));
}

static void	sha256_round(uint32_t *work, uint32_t word)
{
	uint32_t	temp1;
	uint32_t	temp2;
	int			i;

	temp1 = work[7] + big_sigma(work[4], 6, 11, 25)
		+ ((work[4] & work[5]) ^ (~work[4] & work[6])) + word;
	temp2 = big_sigma(work[0], 2, 13, 22)
		+ ((work[0] & work[1]) ^ (work[0] & work[2]) ^ (work[1] & work[2]));
	i = 8;
	while (--i > 0)
		work[i] = work[i - 1];
	work[4] += temp1;
	work[0] = temp1 + temp2;
}

static void	sha256_transform(void *state, const uint8_t *block)
{
	uint32_t	*hash;
	uint32_t	schedule[64];
	uint32_t	work[8];
	int			i;

	hash = state;
	i = -1;
	while (++i < 64)
	{
		if (i < 16)
			schedule[i] = ft_load_bytes(block + i * 4, 4, 1);
		else
			schedule[i] = small_sigma(schedule[i - 2], 17, 19, 10)
				+ schedule[i - 7] + small_sigma(schedule[i - 15], 7, 18, 3)
				+ schedule[i - 16];
	}
	ft_memcpy(work, hash, sizeof(work));
	i = -1;
	while (++i < 64)
		sha256_round(work, g_sha256_k[i] + schedule[i]);
	i = -1;
	while (++i < 8)
		hash[i] += work[i];
}

void	sha256_run(uint32_t *state, const uint8_t *data, size_t len)
{
	t_block_hash	sha;

	sha.block_size = 64;
	sha.length_size = 8;
	sha.big_endian = 1;
	sha.transform = sha256_transform;
	sha.state = state;
	hash_message(&sha, data, len);
}
