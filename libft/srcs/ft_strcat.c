/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frossiny <frossiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 17:32:07 by frossiny          #+#    #+#             */
/*   Updated: 2018/11/06 17:40:41 by frossiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *s1, const char *s2)
{
	char *dst;

	dst = s1 + ft_strlen(s1);
	while (*s2)
		*dst++ = *s2++;
	*dst = '\0';
	return (s1);
}
