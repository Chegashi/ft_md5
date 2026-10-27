/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	main(int argc, char **argv)
{
	const t_command	*cmd;

	if (argc < 2)
	{
		ft_dprintf(2, "usage: %s command [flags] [file/string]\n", PROG_NAME);
		return (1);
	}
	cmd = find_command(argv[1]);
	if (!cmd)
	{
		ft_dprintf(2, "%s: Error: '%s' is an invalid command.\n\n",
			PROG_NAME, argv[1]);
		print_commands(2);
		return (1);
	}
	return (cmd->run(argc - 2, argv + 2, cmd->data));
}
