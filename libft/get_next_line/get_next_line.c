/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:19 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:19 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	read_until_newline(int fd, char **line);
static int	extract_line(char **stash, char **line, char **newline);

int	get_next_line(int fd, char **line)
{
	int	bytes;

	if (fd < 0 || !line || BUFFER_SIZE < 0 || read(fd, NULL, 0))
		return (-1);
	bytes = read_until_newline(fd, line);
	if (bytes > 0)
		return (1);
	return (bytes);
}

static int	read_until_newline(int fd, char **line)
{
	int			bytes;
	static char	*stash = NULL;
	char		*buffer;
	char		*joined;
	char		*newline;

	if (stash == NULL)
		stash = ft_calloc(1, 1);
	buffer = ft_calloc(BUFFER_SIZE + 1, 1);
	newline = ft_strchr(stash, '\n');
	bytes = 1;
	while (!newline && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		joined = ft_strjoin(stash, buffer);
		free(stash);
		stash = joined;
		newline = ft_strchr(stash, '\n');
	}
	free(buffer);
	return (extract_line(&stash, line, &newline));
}

static int	extract_line(char **stash, char **line, char **newline)
{
	char		*joined;

	if (*newline != NULL)
	{
		*line = ft_substr(*stash, 0, (*newline - *stash));
		joined = ft_strdup(*newline + 1);
		free(*stash);
		*stash = joined;
		return (1);
	}
	else
	{
		*line = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
		return (0);
	}
}
