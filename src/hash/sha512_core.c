/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512_core.c                                      :+:      :+:    :+:   */
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
** 0x0b... values have a leading zero only to get past a norminette bug.
*/
static const uint64_t	g_sha512_k[80] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0x0b5c0fbcfec4d3b2f,
	0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
	0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
	0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0x0b00327c898fb213f,
	0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
	0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
	0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
	0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
	0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
	0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
	0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
	0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static uint64_t	big_sigma(uint64_t x, int r1, int r2, int r3)
{
	return (ft_rotr64(x, r1) ^ ft_rotr64(x, r2)
		^ ft_rotr64(x, r3));
}

static uint64_t	small_sigma(uint64_t x, int r1, int r2, int shift)
{
	return (ft_rotr64(x, r1) ^ ft_rotr64(x, r2) ^ (x >> shift));
}

static void	sha512_round(uint64_t *work, uint64_t word)
{
	uint64_t	temp1;
	uint64_t	temp2;
	int			i;

	temp1 = work[7] + big_sigma(work[4], 14, 18, 41)
		+ ((work[4] & work[5]) ^ (~work[4] & work[6])) + word;
	temp2 = big_sigma(work[0], 28, 34, 39)
		+ ((work[0] & work[1]) ^ (work[0] & work[2]) ^ (work[1] & work[2]));
	i = 8;
	while (--i > 0)
		work[i] = work[i - 1];
	work[4] += temp1;
	work[0] = temp1 + temp2;
}

static void	sha512_transform(void *state, const uint8_t *block)
{
	uint64_t	*hash;
	uint64_t	schedule[80];
	uint64_t	work[8];
	int			i;

	hash = state;
	i = -1;
	while (++i < 80)
	{
		if (i < 16)
			schedule[i] = ft_load_bytes(block + i * 8, 8, 1);
		else
			schedule[i] = small_sigma(schedule[i - 2], 19, 61, 6)
				+ schedule[i - 7] + small_sigma(schedule[i - 15], 1, 8, 7)
				+ schedule[i - 16];
	}
	ft_memcpy(work, hash, sizeof(work));
	i = -1;
	while (++i < 80)
		sha512_round(work, g_sha512_k[i] + schedule[i]);
	i = -1;
	while (++i < 8)
		hash[i] += work[i];
}

void	sha512_run(uint64_t *state, const uint8_t *data, size_t len)
{
	t_block_hash	sha;

	sha.block_size = 128;
	sha.length_size = 16;
	sha.big_endian = 1;
	sha.transform = sha512_transform;
	sha.state = state;
	hash_message(&sha, data, len);
}
