/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/14 16:00:13 by frossiny          #+#    #+#             */
/*   Updated: 2019/02/08 18:08:56 by frossiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	handle_ptr_opts(t_arg *arg, char *buf, unsigned int i)
{
	if (i == 0 && arg->precision == -1)
		buf[i++] = '0';
	while (arg->precision > -1 && i < (unsigned)arg->precision)
		buf[i++] = '0';
	buf[i++] = 'x';
	buf[i++] = '0';
	while (!arg->left && i < (unsigned)arg->width)
		buf[i++] = ' ';
	buf[i] = '\0';
	ft_strrev(buf);
	while (arg->left && i < (unsigned)arg->width)
		buf[i++] = ' ';
	buf[i] = '\0';
	arg->str = buf;
}

void	handle_ptr(t_arg *arg)
{
	const char			hex[] = "0123456789abcdef";
	char				*buf;
	size_t				i;

	if (arg->width > 18)
		buf = malloc(sizeof(char) * arg->width + 1);
	else
		buf = malloc(sizeof(char) * 19);
	if (!buf)
	{
		arg->str = ft_strdup("");
		return ;
	}
	i = 0;
	while (arg->u_data.ull > 0)
	{
		buf[i++] = hex[arg->u_data.ull % 16];
		arg->u_data.ull /= 16;
	}
	handle_ptr_opts(arg, buf, i);
}
