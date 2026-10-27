/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hash.h"

const t_hash			g_sha224 = {"sha224", "SHA224", LEN_SHA224, ft_sha224};
const t_hash			g_sha256 = {"sha256", "SHA256", LEN_SHA256, ft_sha256};

static const uint32_t	g_sha224_init[8] = {
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

static const uint32_t	g_sha256_init[8] = {
	0x6a09e667, 0x0bb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

void	ft_sha224(const uint8_t *data, size_t len, uint8_t *digest)
{
	uint32_t	state[8];
	int			i;

	ft_memcpy(state, g_sha224_init, sizeof(state));
	sha256_run(state, data, len);
	i = -1;
	while (++i < LEN_SHA224 / 4)
		ft_store_bytes(digest + i * 4, state[i], 4, 1);
}

void	ft_sha256(const uint8_t *data, size_t len, uint8_t *digest)
{
	uint32_t	state[8];
	int			i;

	ft_memcpy(state, g_sha256_init, sizeof(state));
	sha256_run(state, data, len);
	i = -1;
	while (++i < LEN_SHA256 / 4)
		ft_store_bytes(digest + i * 4, state[i], 4, 1);
}
