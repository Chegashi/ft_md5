/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:20 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:20 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*fill_digits(int len, unsigned int value, char *result)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (value == 10)
		{
			result[i++] = '0';
			result[i++] = '1';
		}
		else
		{
			result[i++] = '0' + (value % 10);
			value /= 10;
		}
	}
	result[i] = '\0';
	return (result);
}

static void	count_digits(int *len, int *sign, unsigned int *value, int n)
{
	*len = 1;
	*sign = 1;
	if (n < 0)
		*sign = -1;
	*value = *sign * n;
	while (*value / 10)
	{
		(*len)++;
		*value /= 10;
	}
	*value = *sign * n;
}

char	*ft_itoa(int n)
{
	int				len;
	int				sign;
	char			*result;
	char			*digits;
	unsigned int	value;

	count_digits(&len, &sign, &value, n);
	result = (char *)malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		digits = result + 1;
		digits = ft_strrev((fill_digits(len, value, digits)));
	}
	else
		result = ft_strrev((fill_digits(len, value, result)));
	return (result);
}
