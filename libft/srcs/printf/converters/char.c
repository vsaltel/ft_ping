/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:22:56 by frossiny          #+#    #+#             */
/*   Updated: 2019/02/08 18:36:32 by frossiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*alloc_opts(t_arg *arg, size_t *len, int r)
{
	char	*new;

	if (arg->precision > -1 && *len > (size_t)arg->precision)
		*len = arg->precision;
	if (r)
	{
		if ((size_t)arg->width < *len)
			new = ft_strnew((size_t)arg->width);
		else
			new = ft_strnew(*len);
	}
	else
	{
		if ((size_t)arg->width > *len)
			new = ft_strnew(arg->width);
		else
			new = ft_strnew(*len);
	}
	return (new);
}

void	handle_modulo(t_arg *arg)
{
	char	*str;
	size_t	i;

	if (arg->width)
		str = ft_strnew(arg->width);
	else
		str = ft_strnew(1);
	if (!str)
		arg->str = ft_strdup("%");
	else
	{
		i = 0;
		while (arg->width && i < (unsigned)arg->width - 1)
			handle_zero_space(arg, str, i++);
		str[i++] = '%';
		str[i] = '\0';
		if (arg->left)
			ft_strrev(str);
		arg->str = str;
	}
}

void	handle_char(t_arg *arg)
{
	size_t	i;

	i = 0;
	if (arg->width > 0)
		arg->str = ft_strnew(arg->width);
	else
		arg->str = ft_strnew(1);
	if (arg->str)
	{
		while (!arg->left && arg->width && i < (unsigned)arg->width - 1)
			handle_zero_space(arg, arg->str, i++);
		arg->str[i++] = (unsigned char)arg->u_data.c;
		while (arg->left && i < (unsigned)arg->width)
			handle_zero_space(arg, arg->str, i++);
	}
	else
		arg->str = ft_strdup("");
}

void	handle_null_str(t_arg *arg)
{
	char		*new;
	size_t		len;
	size_t		i;
	size_t		j;
	const char	nullstr[] = "(null)";

	i = 0;
	j = 0;
	len = 6;
	new = alloc_opts(arg, &len, 1);
	if (new)
	{
		while (!arg->left && i + len < (size_t)arg->width)
			handle_zero_space(arg, new, i++);
		while (nullstr[j] && j < len)
			new[i++] = nullstr[j++];
		while (arg->left && i++ < (size_t)arg->width)
			handle_zero_space(arg, new, i++);
		new[i] = '\0';
	}
	else
		new = ft_strdup("");
	arg->str = new;
}

void	handle_str(t_arg *arg)
{
	char	*str;
	char	*new;
	size_t	len;
	size_t	i;

	str = (char *)arg->u_data.ptr;
	if (!str)
		return (handle_null_str(arg));
	len = ft_strlen(str);
	new = alloc_opts(arg, &len, 0);
	i = 0;
	if (new)
	{
		while (!arg->left && i + len < (size_t)arg->width)
			handle_zero_space(arg, new, i++);
		ft_memcpy(new + i, str, len);
		i += len;
		while (arg->left && i < (unsigned)arg->width)
			handle_zero_space(arg, new, i++);
		new[i] = '\0';
	}
	else
		new = ft_strdup("");
	arg->str = new;
}
