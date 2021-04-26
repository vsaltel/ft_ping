/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 17:28:10 by frossiny          #+#    #+#             */
/*   Updated: 2019/12/10 16:36:34 by vsaltel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*strnfjoin(char *p1, char *p2, int n, char *f)
{
	char	*res;
	int		lp1;

	lp1 = ft_strlen(p1);
	res = malloc(lp1 + n + 1);
	ft_strcpy(res, p1);
	ft_strncpy(res + lp1, p2, n);
	res[lp1 + n] = '\0';
	free(f);
	return (res);
}

static char	*get_buf(char **res, char buf[], t_printf_state *s)
{
	*res = strnfjoin(*res, buf, s->index, *res);
	buf[0] = '\0';
	s->index = 0;
	return (*res);
}

static size_t	get_arg(char **res, char buf[], t_printf_state *s)
{
	size_t	str_len;

	if (!s->arg || s->arg->str == NULL)
		return (s->index);
	str_len = ft_strlen(s->arg->str);
	if (s->arg->type == 'c' && s->arg->u_data.c == 0)
	{
		get_buf(res, buf, s);
		if (s->arg->width == 0)
			*res = strnfjoin(*res, s->arg->str, 1, *res);
		else
			*res = strnfjoin(*res, s->arg->str, s->arg->width, *res);
		return (s->index);
	}
	if (str_len >= BUFF_SIZE)
	{
		get_buf(res, buf, s);
		*res = strnfjoin(*res, s->arg->str, str_len, *res);
		return (s->index);
	}
	if (s->index + ft_strlen(s->arg->str) >= BUFF_SIZE)
		get_buf(res, buf, s);
	ft_strcat(buf + s->index, s->arg->str);
	return ((s->index = ft_strlen(buf)));
}

static char	*get_end(char **res, char buf[], char *format,
	t_printf_state *s)
{
	if (ft_strlen(buf) > 0)
		get_buf(res, buf, s);
	*res = strnfjoin(*res, format, ft_strlen(format), *res);
	return (*res);
}

char	*get_all(int i, char **res, char *format, t_arg *alst)
{
	t_printf_state	s;
	char			buf[BUFF_SIZE + 1];

	s.fd = i;
	i = 0;
	s.arg = alst;
	s.index = 0;
	s.c = 0;
	buf[0] = '\0';
	while (format[i])
	{
		if (s.arg)
		{
			ft_strncat(buf, format + i, s.arg->index - i);
			s.index += s.arg->index - i;
			buf[s.index] = '\0';
			s.index = get_arg(res, buf, &s);
			i = s.arg->end + 1;
			s.arg = s.arg->next;
		}
		else
			return (get_end(res, buf, format + i, &s));
	}
	get_buf(res, buf, &s);
	return (*res);
}
