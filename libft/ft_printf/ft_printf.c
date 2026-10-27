/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochegri <mochegri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 12:18:18 by mochegri          #+#    #+#             */
/*   Updated: 2026/10/21 12:18:18 by mochegri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	g_written;
static int	g_fd;

static int	run_format(int fd, const char *fmt, va_list *ap)
{
	g_written = 0;
	g_fd = fd;
	pf_format((char *)fmt, ap);
	return (g_written);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		printed;

	va_start(args, fmt);
	printed = run_format(1, fmt, &args);
	va_end(args);
	return (printed);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	va_list	args;
	int		printed;

	va_start(args, fmt);
	printed = run_format(fd, fmt, &args);
	va_end(args);
	return (printed);
}

void	pf_putchar(char c)
{
	write(g_fd, &c, 1);
	g_written++;
}

int	pf_written(void)
{
	return (g_written);
}
