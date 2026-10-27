/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "digest.h"

static const t_flag	g_flags[] = {
{"-p", FLAG_P},
{"-q", FLAG_Q},
{"-r", FLAG_R},
{"-s", FLAG_S},
{NULL, 0}
};

static int	lookup_flag(const char *arg)
{
	int	i;

	i = 0;
	while (g_flags[i].str)
	{
		if (!ft_strcmp((char *)g_flags[i].str, (char *)arg))
			return (g_flags[i].bit);
		i++;
	}
	return (0);
}

static int	option_error(t_digest *ctx, const char *msg, const char *opt)
{
	ft_dprintf(2, "%s: %s: %s -- %s\n", PROG_NAME, ctx->hash->name, msg, opt);
	ft_dprintf(2, "usage: %s %s [-pqr] [-s string] [files ...]\n",
		PROG_NAME, ctx->hash->name);
	return (-1);
}

/*
** Returns the index of the first file (flags stop there), or -1.
*/
static int	parse_options(t_digest *ctx, int argc, char **argv)
{
	int	i;
	int	flag;

	i = 0;
	while (i < argc && argv[i][0] == '-' && argv[i][1])
	{
		flag = lookup_flag(argv[i]);
		if (!flag)
			return (option_error(ctx, "illegal option", argv[i] + 1));
		if (flag == FLAG_S && i + 1 >= argc)
			return (option_error(ctx, "option requires an argument", "s"));
		if (flag == FLAG_S)
			i++;
		ctx->flags |= flag;
		i++;
	}
	return (i);
}

int	digest_command(int argc, char **argv, const void *data)
{
	t_digest	ctx;
	int			first_file;
	int			i;

	ctx.hash = data;
	ctx.flags = 0;
	ctx.status = 0;
	first_file = parse_options(&ctx, argc, argv);
	if (first_file < 0)
		return (1);
	if (ctx.flags & FLAG_P)
		digest_stdin(&ctx, 1);
	else if (!(ctx.flags & FLAG_S) && first_file == argc)
		digest_stdin(&ctx, 0);
	i = 0;
	while (i < first_file)
	{
		if (!ft_strcmp(argv[i], "-s"))
			digest_string(&ctx, argv[++i]);
		i++;
	}
	while (first_file < argc)
		digest_file(&ctx, argv[first_file++]);
	return (ctx.status);
}
