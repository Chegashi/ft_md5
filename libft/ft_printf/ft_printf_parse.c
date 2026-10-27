/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	parse_spec(char **fmt, va_list *ap, t_spec *spec)
{
	reset_spec(spec);
	parse_flags(fmt, spec);
	parse_width(fmt, ap, spec);
	parse_precision(fmt, ap, spec);
	parse_qualifier(fmt, spec);
}

void	parse_flags(char **fmt, t_spec *spec)
{
	int	flags;

	flags = 0;
	while (**fmt && char_in_set(**fmt, "-+ #0"))
	{
		if (**fmt == '-')
			flags |= LEFT;
		if (**fmt == ' ')
			flags |= SPACE;
		if (**fmt == '+')
			flags |= PLUS;
		if (**fmt == '#')
			flags |= SPECIAL;
		if (**fmt == '0')
		{
			flags |= ZEROPAD;
			spec->percent_pad = '0';
		}
		++(*fmt);
	}
	spec->flags = flags;
}

void	parse_width(char **fmt, va_list *ap, t_spec *spec)
{
	int	width;

	width = -1;
	if (ft_isdigit(**fmt))
		width = parse_int(fmt);
	else if (**fmt == '*')
	{
		(*fmt)++;
		width = va_arg(*ap, int);
		if (width < 0)
		{
			width = -width;
			spec->flags |= LEFT;
		}
	}
	spec->width = width;
}

void	parse_precision(char **fmt, va_list *ap, t_spec *spec)
{
	int	precision;

	precision = -1;
	if (**fmt == '.')
	{
		spec->flags &= ~ZEROPAD;
		(*fmt)++;
		precision = 0;
		if (ft_isdigit(**fmt))
			precision = parse_int(fmt);
		if (**fmt == '*')
		{
			(*fmt)++;
			precision = va_arg(*ap, int);
		}
		if (precision < 0)
			precision = -1;
	}
	spec->precision = precision;
}

void	parse_qualifier(char **fmt, t_spec *spec)
{
	spec->qualifier = -1;
	if ((**fmt == 'h') || (**fmt == 'l') || (**fmt == 'L'))
		spec->qualifier = *(*fmt)++;
}
