/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 15:43:05 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/29 12:04:39 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n && *str1 != '\0' && *str2 != '\0')
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1 = str1 + 1;
		str2 = str2 + 1;
		i = i + 1;
	}
	if (i < n)
	{
		if (*str1 != '\0' && *str2 == '\0')
			return (*str1 - 0);
		if (*str2 != '\0' && *str1 == '\0')
			return (0 - *str2);
	}
	return (0);
}
