/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeyer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/16 20:55:02 by lmeyer            #+#    #+#             */
/*   Updated: 2016/12/16 21:17:49 by lmeyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char					*ft_strndup(const char *s1, size_t n)
{
	char	*res;
	size_t	i;

	i = ft_strlen(s1);
	if (i > n)
		i = n;
	if (!(res = ft_strnew(i + 1)))
		return (NULL);
	ft_strncpy(res, s1, i);
	return (res);
}
