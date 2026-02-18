/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:13:57 by nmichaud          #+#    #+#             */
/*   Updated: 2025/12/18 15:35:18 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_conditions(const char **format, size_t *nb_chars, va_list *ap)
{
	if (**format == 'i' || **format == 'd')
		*nb_chars += ft_integer((int)va_arg(*ap, int), format);
	else if (**format == 'p')
		*nb_chars += ft_pointer((long long)va_arg(*ap, void *), format);
	else if (**format == 's')
		*nb_chars += ft_string((char *)va_arg(*ap, char *), format);
	else if (**format == 'c')
		*nb_chars += ft_char((int)va_arg(*ap, int), format);
	else if (**format == 'u')
		*nb_chars += ft_unsigned_int((unsigned int)va_arg(*ap, int), format);
	else if (**format == 'x' || **format == 'X')
		*nb_chars += ft_hexa((unsigned int)va_arg(*ap, int), format);
	else
		*nb_chars += ft_char(**format, format);
}	

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	size_t	nb_chars;

	if (format == NULL)
		return (-1);
	va_start(ap, format);
	nb_chars = 0;
	while (*format != 0)
	{
		if (*format == '%' && *(format + 1) == 0)
			return (-1);
		if (*format != '%')
		{
			ft_putchar_fd(*format, 1);
			nb_chars++;
			format++;
			continue ;
		}
		format++;
		ft_conditions(&format, &nb_chars, &ap);
	}
	va_end(ap);
	return (nb_chars);
}

/*#include <stdio.h>
int	main(void)
{
	int	a;
	int	b = 0;

	a = ft_printf("%%0", &a);
	printf("\n");
	b = printf("%%0", &a);
	printf("\nreturn ft: %i return of: %i", a, b);
	return (0);
}*/
