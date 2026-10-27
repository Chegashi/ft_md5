/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hash.h"

const t_hash			g_sha384 = {"sha384", "SHA384", LEN_SHA384, ft_sha384};
const t_hash			g_sha512 = {"sha512", "SHA512", LEN_SHA512, ft_sha512};

static const uint64_t	g_sha384_init[8] = {
	0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17,
	0x152fecd8f70e5939, 0x67332667ffc00b31, 0x8eb44a8768581511,
	0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
};

static const uint64_t	g_sha512_init[8] = {
	0x6a09e667f3bcc908, 0x0bb67ae8584caa73b, 0x3c6ef372fe94f82b,
	0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
	0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

void	ft_sha384(const uint8_t *data, size_t len, uint8_t *digest)
{
	uint64_t	state[8];
	int			i;

	ft_memcpy(state, g_sha384_init, sizeof(state));
	sha512_run(state, data, len);
	i = -1;
	while (++i < LEN_SHA384 / 8)
		ft_store_bytes(digest + i * 8, state[i], 8, 1);
}

void	ft_sha512(const uint8_t *data, size_t len, uint8_t *digest)
{
	uint64_t	state[8];
	int			i;

	ft_memcpy(state, g_sha512_init, sizeof(state));
	sha512_run(state, data, len);
	i = -1;
	while (++i < LEN_SHA512 / 8)
		ft_store_bytes(digest + i * 8, state[i], 8, 1);
}
