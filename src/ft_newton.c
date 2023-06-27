/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_newton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:35:21 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/22 17:31:36 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

double	ft_pow(double val, int n)
{
	double	prod;
	double	div;
	int		i;

	if (val == 0)
		return (0);
	i = 0;
	div = 1;
	if (n < 0)
		div = -1;
	prod = 1;
	while (i != n)
	{
		if (n > 0)
			prod = prod * val;
		else
			prod = prod / val;
		i += div;
	}
	return (prod);
}

int	ft_newton(double val, uint16_t pow, double *out)
{
	double	f;
	double	fp;
	double	xn;
	int8_t	i;

	if (!out || val < 0.0)
		return (-1);
	if (pow == 1 || val == 0)
		return (val);
	xn = val;
	i = -1;
	while (++i < MAX_ITERATIONS)
	{
		f = ft_pow(xn, pow) - val;
		fp = pow * (ft_pow(xn, pow - 1));
		xn = xn - (f / fp);
	}
	*out = xn;
	return (0);
}
