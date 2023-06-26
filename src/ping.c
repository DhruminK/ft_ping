/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:43:08 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 14:44:10 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

int	main(int ac, char **av)
{
	uint8_t		*msg;
	uint32_t	i;
	t_icmp		req_info;
	char		dst_addr[INET_ADDRSTRLEN];
	int			ret;

	if (ac != 2)
		return (1);
	ft_memset(dst_addr, 0, sizeof(char) * INET_ADDRSTRLEN);
	ret = ft_ping_parse_addr(av[1], dst_addr);
	printf("RET: %d : (%s)\n", ret, dst_addr);
	ft_memset(&req_info, 0, sizeof(t_icmp));
	msg = 0;
	req_info.id = 10;
	req_info.seq = 10;
	req_info.data_size = FT_ICMP_DATA_SIZE;
	if (ft_ping_create_icmp_req(&msg, &req_info) == 0)
	{
		i = 0;
		while (i < FT_ICMP_HDR + req_info.data_size)
			printf("%#X ", msg[i++]);
		printf("\n");
	}
	if (msg)
		free(msg);
	if (req_info.data)
		free(req_info.data);
	return (0);
}
