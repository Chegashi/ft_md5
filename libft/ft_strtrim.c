/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:19 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:19 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *s2)
{
	int		j;
	int		start;
	int		end;
	char	*result;

	j = 0;
	if (!s1)
		return (NULL);
	while (is_in_set((char *)s2, s1[j]))
		j++;
	start = j;
	j = ft_strlen(s1);
	while (is_in_set((char *)s2, s1[j - 1]))
		j--;
	end = j;
	if (start > end)
		return (ft_strdup(""));
	result = malloc(sizeof(char) * (end - start + 1));
	if (result == NULL)
		return (NULL);
	j = -1;
	while (++j < (end - start))
		result[j] = s1[start + j];
	result[j] = '\0';
	return (result);
}
