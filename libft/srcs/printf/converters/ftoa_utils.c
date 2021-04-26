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

static void	handle_exceptions_cont(t_arg *arg)
{
	size_t	i;

	if (arg->type == 'f')
		ft_memcpy(arg->str, "nan", 4);
	else
		ft_memcpy(arg->str, "NAN", 4);
	i = 3;
	while (i < (unsigned)arg->width)
		arg->str[i++] = ' ';
	arg->str[i] = '\0';
	if (!arg->left)
		ft_strrev(arg->str);
}

int	handle_exceptions(t_arg *arg)
{
	size_t	len;

	if (is_infinite(arg))
		handle_inf(arg);
	else if (is_nan(arg))
	{
		if (arg->width > 3)
			len = arg->width;
		else
			len = 3;
		arg->str = (char *)malloc(sizeof(char) * (len + 1));
		if (!arg->str)
		{
			arg->str = ft_strdup("");
			return (arg->str != NULL);
		}
		handle_exceptions_cont(arg);
		return (1);
	}
	else
		return (0);
	return (1);
}
