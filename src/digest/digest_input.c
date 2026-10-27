/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_ssl.h"
#include "digest.h"

void	digest_stdin(t_digest *ctx, int echo)
{
	uint8_t	digest[DIGEST_MAX];
	char	*input;
	ssize_t	len;

	len = ft_read_all(0, &input);
	if (len < 0)
		return (print_error(ctx, "stdin"));
	ctx->hash->fn((uint8_t *)input, len, digest);
	if (echo)
		print_echo_result(ctx, digest, input, len);
	else
		print_stdin_result(ctx, digest);
	free(input);
}

void	digest_string(t_digest *ctx, const char *s)
{
	uint8_t	digest[DIGEST_MAX];

	ctx->hash->fn((const uint8_t *)s, ft_strlen(s), digest);
	print_result(ctx, digest, s, 1);
}

void	digest_file(t_digest *ctx, const char *path)
{
	uint8_t	digest[DIGEST_MAX];
	char	*input;
	ssize_t	len;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (print_error(ctx, path));
	len = ft_read_all(fd, &input);
	close(fd);
	if (len < 0)
		return (print_error(ctx, path));
	ctx->hash->fn((uint8_t *)input, len, digest);
	print_result(ctx, digest, path, 0);
	free(input);
}
