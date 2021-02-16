/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcut.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsaltel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 15:57:14 by vsaltel           #+#    #+#             */
/*   Updated: 2019/04/25 15:57:35 by vsaltel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcut(char *str, size_t beg, size_t end)
{
	char	*l;
	char	*r;
	char	*dest;

	if (beg > end)
		return (NULL);
	l = ft_strndup(str, beg);
	if (str[end])
		r = ft_strdup(str + end + 1);
	else
		r = ft_strdup(str + end);
	dest = ft_strjoin(l, r);
	free(l);
	free(r);
	return (dest);
}
