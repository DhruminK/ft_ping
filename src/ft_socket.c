/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_socket.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:04:22 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/27 18:04:23 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

static int	ft_ping_parse_addrinfo(t_icmp_info *info)
{
	struct sockaddr_in	*s_addr;

	if (!info)
		return (-1);
	info->res = info->start_res;
	while (info->res)
	{
		if (info->res->ai_family == AF_INET)
			break ;
		info->res = info->res->ai_next;
	}
	if (!(info->res))
		return (-1);
	s_addr = (struct sockaddr_in *)(info->res->ai_addr);
	if (!(inet_ntop(AF_INET, (void *)&(s_addr->sin_addr),
				info->req_dst_addr, INET_ADDRSTRLEN)))
		return (-1);
	info->req_addr = info->res->ai_addr;
	return (0);
}

static int	ft_parse_host(char *arg, t_icmp_info *info)
{
	struct addrinfo	hints;

	if (!info)
		return (-1);
	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(arg, 0, &hints, &(info->start_res)) < 0)
		return (-1);
	return (ft_ping_parse_addrinfo(info));
}

int	ft_socket_opt(int ttl)
{
	int	sock_fd;

	if (ttl < 0)
		return (-1);
	sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0)
		return (-1);
	if (setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == 0)
		return (sock_fd);
	printf("ft_ping: setsockopt error: %s\n", strerror(errno));
	close(sock_fd);
	return (-1);
}

int	ft_socket_init(char *arg, t_icmp_stats *stats,
		t_icmp_info *info, uint8_t flag)
{
	if (!stats || !info)
		return (-1);
	ft_memset(stats, 0, sizeof(t_icmp_stats));
	ft_memset(info, 0, sizeof(t_icmp_info));
	stats->t_min = 2000;
	stats->t_max = 0;
	stats->uid = getuid();
	stats->arg = arg;
	stats->flag = flag;
	info->arg = arg;
	if (ft_parse_host(arg, info) == -1)
	{
		printf("ft_ping: malloc Error\n");
		return (-1);
	}
	return (ft_socket_opt(FT_IP_TTL));
}
