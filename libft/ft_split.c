/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char sep)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == sep)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static int	word_length(const char *s, char sep)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != sep)
		len++;
	return (len);
}

static void	*free_words(char **words, int count)
{
	while (count--)
		free(words[count]);
	free(words);
	return (NULL);
}

static char	**fill_words(const char *s, char sep, char **words, int count)
{
	int	i;
	int	len;

	i = 0;
	while (i < count)
	{
		while (*s == sep)
			s++;
		len = word_length(s, sep);
		words[i] = ft_substr(s, 0, len);
		if (!words[i])
			return (free_words(words, i));
		s += len;
		i++;
	}
	words[i] = NULL;
	return (words);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	int		count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	words = malloc(sizeof(char *) * (count + 1));
	if (!words)
		return (NULL);
	return (fill_words(s, c, words, count));
}
