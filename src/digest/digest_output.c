/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "ft_ssl.h"
#include "digest.h"

/*
** Strings and files:  MD5 ("foo") = <hex>  |  -r: <hex> "foo"  |  -q: <hex>
*/
void	print_result(t_digest *ctx, const uint8_t *digest,
		const char *name, int quoted)
{
	char		hex[DIGEST_MAX * 2 + 1];
	const char	*quote;

	ft_bytes_to_hex(digest, ctx->hash->digest_len, hex);
	quote = "";
	if (quoted)
		quote = "\"";
	if (ctx->flags & FLAG_Q)
		ft_printf("%s\n", hex);
	else if (ctx->flags & FLAG_R)
		ft_printf("%s %s%s%s\n", hex, quote, name, quote);
	else
		ft_printf("%s (%s%s%s) = %s\n", ctx->hash->label,
			quote, name, quote, hex);
}

/*
** stdin without -p:  (stdin)= <hex>  |  -r: <hex> *stdin  |  -q: <hex>
*/
void	print_stdin_result(t_digest *ctx, const uint8_t *digest)
{
	char	hex[DIGEST_MAX * 2 + 1];

	ft_bytes_to_hex(digest, ctx->hash->digest_len, hex);
	if (ctx->flags & FLAG_Q)
		ft_printf("%s\n", hex);
	else if (ctx->flags & FLAG_R)
		ft_printf("%s *stdin\n", hex);
	else
		ft_printf("(stdin)= %s\n", hex);
}

/*
** stdin with -p:  ("<input>")= <hex>  |  -q: <input><hex>
*/
void	print_echo_result(t_digest *ctx, const uint8_t *digest,
		const char *input, size_t len)
{
	char	hex[DIGEST_MAX * 2 + 1];

	ft_bytes_to_hex(digest, ctx->hash->digest_len, hex);
	if (ctx->flags & FLAG_Q)
	{
		write(1, input, len);
		ft_printf("%s\n", hex);
		return ;
	}
	if (len > 0 && input[len - 1] == '\n')
		len--;
	ft_printf("(\"");
	write(1, input, len);
	ft_printf("\")= %s\n", hex);
}

void	print_error(t_digest *ctx, const char *name)
{
	ft_dprintf(2, "%s: %s: %s: %s\n", PROG_NAME, ctx->hash->name, name,
		strerror(errno));
	ctx->status = 1;
}
