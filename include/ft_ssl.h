/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:16 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "libft.h"

# define PROG_NAME "ft_ssl"

typedef int	(*t_cmd_fn)(int argc, char **argv, const void *data);

typedef struct s_command
{
	const char	*name;
	t_cmd_fn	run;
	const void	*data;
}	t_command;

const t_command	*find_command(const char *name);
void			print_commands(int fd);

#endif
