/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "digest.h"
#include "hash.h"

static const t_command	g_commands[] = {
{"md5", digest_command, &g_md5},
{"sha224", digest_command, &g_sha224},
{"sha256", digest_command, &g_sha256},
{"sha384", digest_command, &g_sha384},
{"sha512", digest_command, &g_sha512},
{"whirlpool", digest_command, &g_whirlpool},
{NULL, NULL, NULL}
};

const t_command	*find_command(const char *name)
{
	int	i;

	i = 0;
	while (g_commands[i].name)
	{
		if (!ft_strcmp((char *)g_commands[i].name, (char *)name))
			return (&g_commands[i]);
		i++;
	}
	return (NULL);
}

void	print_commands(int fd)
{
	int	i;

	ft_dprintf(fd, "Commands:\n");
	i = 0;
	while (g_commands[i].name)
		ft_dprintf(fd, "%s\n", g_commands[i++].name);
	ft_dprintf(fd, "\nFlags:\n-p -q -r -s\n");
}
