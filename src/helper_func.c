/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:43:02 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 14:43:03 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

void	*ft_memcpy(void *dst, void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	i = 0;
	d = (char *)dst;
	s = (char *)src;
	while (++i <= n)
		d[i - 1] = s[i - 1];
	return (dst);
}

void	*ft_memset(void *b, int c, size_t n)
{
	char	*d;
	size_t	i;

	i = 0;
	d = (char *)b;
	while (++i <= n)
		d[i - 1] = (unsigned char)c;
	return (b);
}
