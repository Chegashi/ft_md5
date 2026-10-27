/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	pf_format(char *fmt, va_list *ap)
{
	t_spec	spec;

	while (*fmt)
	{
		if (*fmt != '%')
		{
			pf_putchar(*(fmt++));
			continue ;
		}
		++fmt;
		parse_spec(&fmt, ap, &spec);
		if (*fmt == 'n')
			store_written_count(&spec, ap);
		if (*fmt && char_in_set(*fmt, "cspdiuxX%"))
		{
			print_conversion(&fmt, &spec, ap);
			continue ;
		}
		if (*fmt)
			++fmt;
	}
}

void	reset_spec(t_spec *spec)
{
	spec->flags = 0;
	spec->width = -1;
	spec->precision = -1;
	spec->base = 10;
	spec->qualifier = -1;
	spec->sign = 0;
	spec->pad = ' ';
	spec->percent_pad = ' ';
	spec->lowercase = 1;
	spec->num = 0;
}

int	parse_int(char **str)
{
	int	value;

	value = 0;
	while (ft_isdigit(**str))
		value = value * 10 + (*(*str)++) - '0';
	return (value);
}

int	char_in_set(char c, char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

/*
** Makes room in the width for the "0x" / "0X" prefix of the # flag.
*/
void	reserve_prefix_width(t_spec *spec)
{
	if ((spec->flags & SPECIAL) && spec->base == 16)
		spec->width -= 2;
}
