/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:17 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:17 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*tmp;
	char	locate;

	locate = (char)c;
	tmp = (char *)str;
	i = 0;
	while (tmp[i] != '\0')
	{
		if (tmp[i] == locate)
			return (tmp + i);
		else
			i++;
	}
	if (tmp[i] == '\0' && locate == '\0')
		return (tmp + i);
	return (NULL);
}
