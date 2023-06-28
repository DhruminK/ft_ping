/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:04:28 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/27 18:04:29 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

static int	ft_start_ping(int sock_fd, t_icmp_stats *stats, t_icmp_info *info)
{
	if (!stats || !info)
		return (-1);
	signal(SIGALRM, signalhandler);
	signal(SIGINT, signalhandler);
	info->req.data_size = FT_ICMP_DATA_SIZE;
	ft_print_ping_info(info, stats->flag);
	g_sig_handler = SIGALRM;
	while (g_sig_handler != SIGINT)
	{
		if (ft_ping_once(sock_fd, stats, info) == -1)
			return (-1);
	}
	ft_print_stats(stats);
	return (0);
}

static int	ft_error_ret(int err_no, int ret)
{
	if (err_no == 64)
		printf("ft_ping: unkown host\n");
	if (err_no == 0)
		printf("ft_ping: usage error: need to be run as root\n");
	return (ret);
}

int	main(int ac, char **av)
{
	t_icmp_stats	stats;
	t_icmp_info		info;
	int				sock_fd;
	int				ret;

	if (ac != 2)
		return (64);
	if (getuid() != 0)
		return (ft_error_ret(0, 0));
	sock_fd = ft_socket_init(av[1], &stats, &info, FT_PING_VERBOSE);
	if (sock_fd < 0)
		return (ft_error_ret(64, 64));
	ret = ft_start_ping(sock_fd, &stats, &info);
	close(sock_fd);
	if (ret < 0 || stats.num_packets_recv > 0)
		ret = 1;
	return (ret);
}
