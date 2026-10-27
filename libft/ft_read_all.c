/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:15 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:15 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define READ_SIZE 4096

static char	*grow_buffer(char *buffer, size_t len, size_t *capacity);

/*
** Reads fd until EOF (binary safe). Returns the length, or -1 on error.
*/
ssize_t	ft_read_all(int fd, char **out)
{
	char	*buffer;
	size_t	len;
	size_t	capacity;
	ssize_t	bytes;

	capacity = READ_SIZE;
	len = 0;
	buffer = malloc(capacity);
	bytes = 1;
	while (buffer && bytes > 0)
	{
		bytes = read(fd, buffer + len, capacity - len);
		if (bytes > 0)
			len += bytes;
		if (len == capacity)
			buffer = grow_buffer(buffer, len, &capacity);
	}
	if (!buffer || bytes < 0)
	{
		free(buffer);
		return (-1);
	}
	*out = buffer;
	return (len);
}

static char	*grow_buffer(char *buffer, size_t len, size_t *capacity)
{
	char	*bigger;

	bigger = malloc(*capacity * 2);
	if (bigger)
		ft_memcpy(bigger, buffer, len);
	free(buffer);
	*capacity *= 2;
	return (bigger);
}
