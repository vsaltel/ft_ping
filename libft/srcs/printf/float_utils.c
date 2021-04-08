/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 15:31:08 by frossiny          #+#    #+#             */
/*   Updated: 2019/02/13 13:34:44 by frossiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	is_infinite(t_arg *arg)
{
	long	expo;
	long	mantissa;

	if (arg->size == L)
		expo = arg->u_data.bin >> 64;
	else
		expo = arg->u_data.bin >> 52;
	if (arg->size == L)
		expo &= 0x7FFF;
	else
		expo &= 0x7FF;
	mantissa = arg->u_data.bin;
	if (arg->size == L)
		mantissa &= 0xFFFFFFFFFFFFFFFF;
	else
		mantissa &= 0xFFFFFFFFFFFFF;
	if (arg->size == L)
		return (expo == 0x7FFF && mantissa == 0);
	else
		return (expo == 0x7FF && mantissa == 0);
}

int	is_nan(t_arg *arg)
{
	long	expo;
	long	mantissa;

	if (arg->size == L)
		expo = arg->u_data.bin >> 64;
	else
		expo = arg->u_data.bin >> 52;
	if (arg->size == L)
		expo &= 0x7FFF;
	else
		expo &= 0x7FF;
	mantissa = arg->u_data.bin;
	if (arg->size == L)
		mantissa &= 0xFFFFFFFFFFFFFFFF;
	else
		mantissa &= 0xFFFFFFFFFFFFF;
	if (arg->size == L)
		return (expo == 0x7FFF && mantissa);
	else
		return (expo == 0x7FF && mantissa);
}

int	is_float_neg(t_arg *arg)
{
	if (arg->size == L)
		return ((arg->u_data.bin >> 79) & 1);
	return ((arg->u_data.bin >> 63) & 1);
}

__int128_t	fround(long double d, int precision)
{
	while (precision--)
		d *= 10;
	if (d > 0)
		d += 0.5;
	else
		d += -0.5;
	return ((__int128_t)d);
}

void	handle_inf(t_arg *arg)
{
	size_t	len;
	size_t	i;
	int		neg;

	neg = is_float_neg(arg);
	if (arg->width > 3 + neg)
		len = arg->width;
	else
		len = 3 + neg;
	arg->str = (char *)malloc(sizeof(char) * (len + 1));
	if (!arg->str)
		return ;
	i = 0;
	while (i < len - (3 + neg) && !arg->left)
		arg->str[i++] = ' ';
	if (neg)
		ft_memcpy(arg->str + i, arg->type == 'F' ? "-INF" : "-inf", 4);
	else
		ft_memcpy(arg->str + i, arg->type == 'F' ? "INF" : "inf", 3);
	i += 3 + neg;
	while (i < (size_t)arg->width && arg->left)
		arg->str[i++] = ' ';
	arg->str[i] = '\0';
}
