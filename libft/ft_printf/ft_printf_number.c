/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_number.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	read_number(t_spec *spec, va_list *ap)
{
	if (spec->qualifier == 'l')
		spec->num = va_arg(*ap, unsigned long);
	else if (spec->qualifier == 'h')
	{
		spec->num = (unsigned short)va_arg(*ap, int);
		if (spec->flags & SIGN)
			spec->num = (short)spec->num;
	}
	else if (spec->flags & SIGN)
		spec->num = va_arg(*ap, int);
	else
		spec->num = va_arg(*ap, unsigned int);
	if (spec->num == 0)
		spec->flags &= ~SPECIAL;
	spec->lowercase = ((spec->flags & SMALL) != 0);
	if (spec->flags & LEFT)
		spec->flags &= ~ZEROPAD;
	spec->pad = ' ';
	if (spec->flags & ZEROPAD)
		spec->pad = '0';
	set_sign(spec);
	reserve_prefix_width(spec);
}

void	print_number(t_spec *spec)
{
	int		len;
	char	digits[66];

	number_to_digits(digits, spec);
	len = ft_strlen(digits);
	if (len > spec->precision && (spec->num != 0 || spec->precision != 0))
		spec->precision = len;
	spec->width -= spec->precision;
	if (!(spec->flags & (ZEROPAD + LEFT)))
		while (spec->width-- > 0)
			pf_putchar(' ');
	if (spec->sign)
		pf_putchar(spec->sign);
	print_prefix_and_pad(spec);
	while (len < spec->precision)
	{
		pf_putchar('0');
		(spec->precision)--;
	}
	if (spec->num != 0 || spec->precision != 0)
		while (len-- > 0)
			pf_putchar(digits[len]);
	while (spec->width-- > 0)
		pf_putchar(' ');
}

/*
** Writes the digits of spec->num in reverse order (least significant first).
*/
char	*number_to_digits(char *out, t_spec *spec)
{
	const char		*digits;
	unsigned long	nbr;
	int				i;

	nbr = (unsigned long)spec->num;
	digits = "0123456789ABCDEF";
	if (spec->lowercase)
		digits = "0123456789abcdef";
	i = 0;
	if (!nbr)
		out[i++] = '0';
	while (nbr)
	{
		out[i++] = digits[nbr % spec->base];
		nbr /= spec->base;
	}
	out[i] = '\0';
	return (out);
}

void	set_sign(t_spec *spec)
{
	spec->sign = 0;
	if (!(spec->flags & SIGN))
		return ;
	if (spec->num < 0)
	{
		spec->sign = '-';
		spec->num *= -1;
	}
	else if (spec->flags & PLUS)
		spec->sign = '+';
	else if (spec->flags & SPACE)
		spec->sign = ' ';
	if (spec->sign)
		spec->width--;
}

void	print_prefix_and_pad(t_spec *spec)
{
	if (spec->flags & SPECIAL)
	{
		pf_putchar('0');
		if (spec->flags & SMALL)
			pf_putchar('x');
		else
			pf_putchar('X');
	}
	if (!(spec->flags & LEFT))
		while ((spec->width)-- > 0)
			pf_putchar(spec->pad);
}
