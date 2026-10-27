/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:16 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:16 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*empty_case_result(const char *str, const char *to_find, size_t len)
{
	if (!*to_find)
		return ((char *)str);
	if (len == 0)
		return (0);
	if (str[0] == '\0' || str == NULL)
		return (NULL);
	return (0);
}

static int	match_at(const char *str, int k, const char *to_find, size_t len)
{
	int	j;

	j = 0;
	while (str[k] == to_find[j] && k < (int)len && str[k] && to_find[j])
	{
		j++;
		k++;
	}
	return (to_find[j] == '\0');
}

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	int		i;

	i = -1;
	if (!*to_find || len == 0 || str[0] == '\0' || str == NULL)
		return (empty_case_result(str, to_find, len));
	while (str[++i])
	{
		if (str[i] == to_find[0] && match_at(str, i, to_find, len))
			return ((char *)str + i);
	}
	if (str[0] == '\0' || *to_find == '\0')
		return ((char *)str);
	return (NULL);
}
