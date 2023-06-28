/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:47:53 by dkhatri           #+#    #+#             */
/*   Updated: 2018/11/21 14:07:02 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_atoi(char *str)
{
	int		sum;
	int		i;
	int		is_negative;

	sum = 0;
	i = 0;
	is_negative = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str = str + 1;
	if (*str == '-' || *str == '+')
		if (*(str++) == '-')
			is_negative = 1;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
		sum = (sum * 10) + (str[i++] - '0');
	return (is_negative == 1 ? sum * -1 : sum);
}
