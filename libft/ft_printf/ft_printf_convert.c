/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_convert.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_conversion(char **fmt, t_spec *spec, va_list *ap)
{
	spec->base = 10;
	if (**fmt == 'c' || **fmt == '%')
		print_char(spec, ap, **fmt);
	else if (**fmt == 's')
		print_string(spec, ap);
	else if (**fmt == 'p')
		print_pointer(spec, ap);
	else if (char_in_set(**fmt, "dxXiu"))
	{
		if (**fmt == 'x')
			spec->flags |= SMALL;
		if (**fmt == 'x' || **fmt == 'X')
			spec->base = 16;
		if (**fmt == 'i' || **fmt == 'd')
			spec->flags |= SIGN;
		read_number(spec, ap);
		print_number(spec);
	}
	(*fmt)++;
}

void	print_char(t_spec *spec, va_list *ap, char conversion)
{
	if (!(spec->flags & LEFT))
	{
		while (--(spec->width) > 0)
		{
			if (conversion == '%')
				pf_putchar(spec->percent_pad);
			else
				pf_putchar(' ');
		}
	}
	if (conversion == '%')
		pf_putchar('%');
	else
		pf_putchar((unsigned char)va_arg(*ap, int));
	while (--(spec->width) > 0)
		pf_putchar(' ');
}

void	print_string(t_spec *spec, va_list *ap)
{
	char	*str;
	int		len;
	int		i;

	str = va_arg(*ap, char *);
	if (str == NULL)
		str = "(null)";
	len = ft_strlen(str);
	if ((unsigned long)len > (unsigned long)spec->precision)
		len = spec->precision;
	if (!(spec->flags & LEFT))
		while (len < spec->width--)
			pf_putchar(' ');
	i = 0;
	while (i < len)
		pf_putchar(str[i++]);
	while (len < spec->width--)
		pf_putchar(' ');
}

void	print_pointer(t_spec *spec, va_list *ap)
{
	spec->flags |= SPECIAL | SMALL;
	spec->lowercase = 1;
	if (spec->width == -1)
		spec->flags |= ZEROPAD;
	spec->base = 16;
	spec->num = va_arg(*ap, size_t);
	spec->width -= 2;
	spec->pad = ' ';
	if (spec->flags & ZEROPAD)
		spec->pad = '0';
	print_number(spec);
}

void	store_written_count(t_spec *spec, va_list *ap)
{
	if (spec->qualifier == 'l')
		*va_arg(*ap, long *) = pf_written();
	else
		*va_arg(*ap, int *) = pf_written();
}
