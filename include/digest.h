/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIGEST_H
# define DIGEST_H

# include <stddef.h>
# include <stdint.h>

# define DIGEST_MAX 64

# define FLAG_P 0x1
# define FLAG_Q 0x2
# define FLAG_R 0x4
# define FLAG_S 0x8

typedef void	(*t_hash_fn)(const uint8_t *data, size_t len, uint8_t *digest);

typedef struct s_hash
{
	const char	*name;
	const char	*label;
	size_t		digest_len;
	t_hash_fn	fn;
}	t_hash;

typedef struct s_flag
{
	const char	*str;
	int			bit;
}	t_flag;

typedef struct s_digest
{
	const t_hash	*hash;
	int				flags;
	int				status;
}	t_digest;

int		digest_command(int argc, char **argv, const void *data);

void	digest_stdin(t_digest *ctx, int echo);
void	digest_string(t_digest *ctx, const char *s);
void	digest_file(t_digest *ctx, const char *path);

void	print_result(t_digest *ctx, const uint8_t *digest,
			const char *name, int quoted);
void	print_stdin_result(t_digest *ctx, const uint8_t *digest);
void	print_echo_result(t_digest *ctx, const uint8_t *digest,
			const char *input, size_t len);
void	print_error(t_digest *ctx, const char *name);

#endif
