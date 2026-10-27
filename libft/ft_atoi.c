/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:19 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:19 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	parse_sign(char c, int *i, int *sign)
{
	if (c == '-' || c == '+')
	{
		if (c == '-')
			*sign = -1;
		*i = (*i + 1);
	}
}

static int	ft_skip_spaces(const char *str, int *i)
{
	while (str[*i] >= 0 && str[*i] <= 32)
	{
		if (str[*i] == '\e' || str[*i] == '\0')
			return (0);
		(*i)++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int				i;
	unsigned int	result;
	int				sign;

	sign = 1;
	result = 0;
	i = 0;
	if (!ft_skip_spaces(str, &i))
		return (0);
	parse_sign(str[i], &i, &sign);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (result * sign);
		result = result + (str[i] - '0');
		if (str[i + 1] >= '0' && str[i + 1] <= '9')
			result *= 10;
		i++;
	}
	if (result > 2147483648)
		return (-(sign != -1));
	return (sign * result);
}
