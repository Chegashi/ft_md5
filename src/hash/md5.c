/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hash.h"

const t_hash			g_md5 = {"md5", "MD5", LEN_MD5, ft_md5};

static const uint32_t	g_md5_init[4] = {
	0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

static const uint32_t	g_md5_sines[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const int		g_md5_shifts[16] = {
	7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21
};

static uint32_t	md5_mix(int step, uint32_t b, uint32_t c, uint32_t d)
{
	if (step < 16)
		return ((b & c) | (~b & d));
	if (step < 32)
		return ((b & d) | (c & ~d));
	if (step < 48)
		return (b ^ c ^ d);
	return (c ^ (b | ~d));
}

static int	md5_word_index(int step)
{
	if (step < 16)
		return (step);
	if (step < 32)
		return ((5 * step + 1) % 16);
	if (step < 48)
		return ((3 * step + 5) % 16);
	return ((7 * step) % 16);
}

static void	md5_transform(void *state, const uint8_t *block)
{
	uint32_t	*hash;
	uint32_t	abcd[4];
	uint32_t	sum;
	int			step;

	hash = state;
	ft_memcpy(abcd, hash, sizeof(abcd));
	step = -1;
	while (++step < 64)
	{
		sum = abcd[0] + md5_mix(step, abcd[1], abcd[2], abcd[3])
			+ g_md5_sines[step]
			+ (uint32_t)ft_load_bytes(block + md5_word_index(step) * 4, 4, 0);
		abcd[0] = abcd[3];
		abcd[3] = abcd[2];
		abcd[2] = abcd[1];
		abcd[1] += ft_rotl32(sum, g_md5_shifts[step / 16 * 4 + step % 4]);
	}
	step = -1;
	while (++step < 4)
		hash[step] += abcd[step];
}

void	ft_md5(const uint8_t *data, size_t len, uint8_t *digest)
{
	t_block_hash	md5;
	uint32_t		state[4];
	int				i;

	ft_memcpy(state, g_md5_init, sizeof(state));
	md5.block_size = 64;
	md5.length_size = 8;
	md5.big_endian = 0;
	md5.transform = md5_transform;
	md5.state = state;
	hash_message(&md5, data, len);
	i = -1;
	while (++i < 4)
		ft_store_bytes(digest + i * 4, state[i], 4, 0);
}
