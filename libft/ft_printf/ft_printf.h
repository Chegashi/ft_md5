/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

# define ZEROPAD	1
# define SIGN		2
# define PLUS		4
# define SPACE		8
# define LEFT		16
# define SMALL		32
# define SPECIAL	64

typedef struct s_spec
{
	int		flags;
	int		width;
	int		precision;
	int		base;
	char	qualifier;
	char	sign;
	char	pad;
	char	percent_pad;
	int		lowercase;
	long	num;
}	t_spec;

void	pf_format(char *fmt, va_list *ap);
void	pf_putchar(char c);
int		pf_written(void);

void	parse_spec(char **fmt, va_list *ap, t_spec *spec);
void	parse_flags(char **fmt, t_spec *spec);
void	parse_width(char **fmt, va_list *ap, t_spec *spec);
void	parse_precision(char **fmt, va_list *ap, t_spec *spec);
void	parse_qualifier(char **fmt, t_spec *spec);

void	print_conversion(char **fmt, t_spec *spec, va_list *ap);
void	print_char(t_spec *spec, va_list *ap, char conversion);
void	print_string(t_spec *spec, va_list *ap);
void	print_pointer(t_spec *spec, va_list *ap);
void	store_written_count(t_spec *spec, va_list *ap);

void	read_number(t_spec *spec, va_list *ap);
void	print_number(t_spec *spec);
char	*number_to_digits(char *out, t_spec *spec);
void	set_sign(t_spec *spec);
void	print_prefix_and_pad(t_spec *spec);

void	reset_spec(t_spec *spec);
int		parse_int(char **str);
int		char_in_set(char c, char *set);
void	reserve_prefix_width(t_spec *spec);

#endif
