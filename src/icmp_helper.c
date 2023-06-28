/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:35:38 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 14:26:43 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

void	ft_icmp_cadd16(uint16_t *c1, uint16_t c2)
{
	uint32_t	c;

	if (!c1 || !c2)
		return ;
	c = (*c1) + c2;
	*c1 = (c & ((0x1 << 16) - 1));
	if (c & (0x1 << 16))
		*c1 += 1;
}

void	ft_icmp_checksum(uint8_t *buf, size_t buf_size, uint16_t *checksum)
{
	size_t		i;
	uint16_t	ret;
	uint16_t	ret1;

	if (!buf || !buf_size || !checksum)
		return ;
	*checksum = 0;
	ret1 = 0;
	i = 0;
	while (i < buf_size)
	{
		ret = 0;
		if (i != buf_size - 1)
			ret = buf[i + 1];
		ret = (((uint16_t)buf[i]) << 8) | ret;
		ft_icmp_cadd16(&ret1, ret);
		i += 2;
	}
	ret1 = ~ret1;
	*checksum = htons(ret1);
}

void	ft_ping_64bit_little_endian(uint8_t *msg, uint64_t val, uint8_t num_bytes)
{
	int		i;
	uint8_t	*v;

	if (!msg || (num_bytes < 2 || num_bytes >8))
		return ;
	v = (uint8_t *)&val;
	if (BYTE_ORDER == BIG_ENDIAN)
		ft_memcpy(msg, v, sizeof(uint8_t) * num_bytes);
	else
	{
		i = -1;
		while (++i < num_bytes)
			msg[num_bytes - 1 - i] = v[i];
	}
}
