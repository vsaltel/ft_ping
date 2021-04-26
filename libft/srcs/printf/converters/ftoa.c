/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 17:06:42 by frossiny          #+#    #+#             */
/*   Updated: 2019/02/13 14:36:41 by frossiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

__int128_t	numtoarg(char buf[], __int128_t n, size_t d)
{
	size_t	i;

	i = 0;
	while (n)
	{
		buf[i++] = '0' + n % 10;
		n /= 10;
	}
	while (i < d)
		buf[i++] = '0';
	buf[i] = '\0';
	ft_strrev(buf);
	return (i);
}

size_t	malloc_str(t_arg *arg, size_t *i, char *buf, int neg)
{
	size_t	len;

	len = *i;
	if ((size_t)arg->width > *i)
		len = (size_t)arg->width;
	if (len == *i)
		len += (arg->positive || arg->space || neg)
			+ (arg->prefix && arg->precision == 0);
	arg->str = (char *)malloc(sizeof(char) * (len + 1));
	if (!arg->str)
	{
		arg->str = ft_strdup("");
		return (0);
	}
	ft_memcpy(arg->str, buf, *i);
	if (arg->prefix && arg->precision == 0)
		arg->str[(*i)++] = '.';
	arg->str[*i] = '\0';
	return (len);
}

void	pad(char *buf, size_t i, t_arg *arg, int neg)
{
	size_t	len;

	len = malloc_str(arg, &i, buf, neg);
	if (!len)
		return ;
	ft_strrev(arg->str);
	while (i < len - (arg->positive || arg->space || neg) && arg->zero)
		arg->str[i++] = '0';
	if (neg)
		arg->str[i++] = '-';
	else if (arg->positive)
		arg->str[i++] = '+';
	else if (arg->space)
		arg->str[i++] = ' ';
	arg->str[i] = '\0';
	if (arg->left)
		ft_strrev(arg->str);
	while (i < (unsigned)arg->width && !arg->zero)
		arg->str[i++] = ' ';
	arg->str[i] = '\0';
	if (!arg->left)
		ft_strrev(arg->str);
}

void	prec_numtoarg(t_arg *arg, char buf[], size_t *i, long double d)
{
	if (d >= 1 && !arg->precision)
		*i += numtoarg(buf + *i, fround(d, 0), 0);
	else if (d >= 1 && arg->precision)
		*i += numtoarg(buf + *i, fround((__int128_t)d, 0), 0);
	else
	{
		buf[*i] = '0';
		(*i)++;
	}
}

void	handle_float(t_arg *arg)
{
	char		buf[128];
	size_t		i;
	long double	d;

	if (handle_exceptions(arg))
		return ;
	i = 0;
	if (arg->size == L)
		d = arg->u_data.ld;
	else
		d = (long double)arg->u_data.d;
	if (is_float_neg(arg))
		d *= -1;
	prec_numtoarg(arg, buf, &i, d);
	if (arg->precision > 0)
	{
		buf[i++] = '.';
		i += numtoarg(buf + i,
				fround((d - (__int128_t)d), arg->precision), arg->precision);
	}
	buf[i] = '\0';
	pad(buf, i, arg, is_float_neg(arg));
}
