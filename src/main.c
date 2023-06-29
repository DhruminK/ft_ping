/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:04:28 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/29 13:38:44 by dkhatri          ###   ########.fr       */
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
	if (info->start_res)
		freeaddrinfo(info->start_res);
	return (0);
}

static int	ft_print_help(void)
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf("Options valid for all request types:\n");
	printf(" --ttl=N\t\tspecify N as time-to-live\n");
	printf(" -q\t\t\tquiet output\n");
	printf(" -?, --help\t\tgive this help list\n");
	return (0);
}

int	main(int ac, char **av)
{
	t_icmp_stats	stats;
	t_icmp_info		info;
	int				sock_fd;
	int				ret;

	if (getuid() != 0)
		return (ft_print_arg_error(FT_ROOT_PER_REQ, 0));
	if (ft_parse_args(ac - 1, av + 1, &stats) < 0)
		return (1);
	if (stats.flag & FT_PING_HELP)
		return (ft_print_help());
	sock_fd = ft_socket_init(&stats, &info);
	if (sock_fd < 0)
		return (ft_print_arg_error(FT_UNKOWN_HOST, 0));
	ret = ft_start_ping(sock_fd, &stats, &info);
	close(sock_fd);
	if (ret < 0 || stats.num_packets_recv > 0)
		ret = 1;
	return (ret);
}
