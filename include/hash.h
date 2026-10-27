/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:16 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/26 12:00:00 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
# define HASH_H

# include "digest.h"

# define LEN_MD5		16
# define LEN_SHA224		28
# define LEN_SHA256		32
# define LEN_SHA384		48
# define LEN_SHA512		64
# define LEN_WHIRLPOOL	64

# define MAX_BLOCK		128

extern const t_hash	g_md5;
extern const t_hash	g_sha224;
extern const t_hash	g_sha256;
extern const t_hash	g_sha384;
extern const t_hash	g_sha512;
extern const t_hash	g_whirlpool;

typedef void		(*t_transform)(void *state, const uint8_t *block);

typedef struct s_block_hash
{
	size_t		block_size;
	size_t		length_size;
	int			big_endian;
	t_transform	transform;
	void		*state;
}	t_block_hash;

void		ft_md5(const uint8_t *data, size_t len, uint8_t *digest);
void		ft_sha224(const uint8_t *data, size_t len, uint8_t *digest);
void		ft_sha256(const uint8_t *data, size_t len, uint8_t *digest);
void		ft_sha384(const uint8_t *data, size_t len, uint8_t *digest);
void		ft_sha512(const uint8_t *data, size_t len, uint8_t *digest);
void		ft_whirlpool(const uint8_t *data, size_t len, uint8_t *digest);

void		hash_message(t_block_hash *hash, const uint8_t *data, size_t len);
void		sha256_run(uint32_t *state, const uint8_t *data, size_t len);
void		sha512_run(uint64_t *state, const uint8_t *data, size_t len);

#endif
