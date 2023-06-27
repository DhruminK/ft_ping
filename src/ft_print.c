/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:03:02 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/27 18:03:21 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

static void	ft_print_destination_unreachable(uint8_t code)
{
	if (code == ICMP_NET_UNREACH)
		printf("Destination Net Unreachable\n");
	else if (code == ICMP_HOST_UNREACH)
		printf("Destination Host Unreachable\n");
	else if (code == ICMP_PROT_UNREACH)
		printf("Destination Protocol Unreachable\n");
	else if (code == ICMP_PORT_UNREACH)
		printf("Destination Port Unreachable\n");
	else if (code == ICMP_FRAG_NEEDED)
		printf("Frag needed and DF set\n");
	else if (code == ICMP_SR_FAILED)
		printf("Source Route Failed\n");
	else
		printf("Destination Unreachable, Bad Code: %d\n", code);
}

static void	ft_print_redirect(uint8_t code)
{
	if (code == ICMP_REDIR_NET)
		printf("Redirect Network\n");
	else if (code == ICMP_REDIR_HOST)
		printf("Redirect Host\n");
	else if (code == ICMP_REDIR_NETTOS)
		printf("Redirect Type of Service and Network\n");
	else if (code == ICMP_REDIR_HOSTTOS)
		printf("Redirect Type of Service and Host\n");
	else
		printf("Redirect, Bad Code: %d\n", code);
}

static void	ft_print_error_helper(uint8_t type, uint8_t code)
{
	if (type == ICMP_TIME_EXCEEDED && code == ICMP_EXC_TTL)
		printf("Time of live exceeded\n");
	else if (type == ICMP_TIME_EXCEEDED && code == ICMP_EXC_FRAGTIME)
		printf("Frag reassembly time exceeded\n");
	else if (type == ICMP_TIME_EXCEEDED)
		printf("Time exceeded, Bad Code: %d\n", code);
	else if (type == ICMP_PARAMETERPROB)
		printf("Parameter Problem\n");
	else if (type == ICMP_SOURCE_QUENCH)
		printf("Source Quench\n");
	else if (type == ICMP_TIMESTAMP)
		printf("Timestamp\n");
	else if (type == ICMP_TIMESTAMPREPLY)
		printf("Timestamp Reply\n");
	else if (type == ICMP_INFO_REQUEST)
		printf("Information Request\n");
	else if (type == ICMP_INFO_REPLY)
		printf("Information Reply\n");
	else if (type == ICMP_REDIRECT)
		ft_print_redirect(code);
	else if (type == ICMP_DEST_UNREACH)
		ft_print_destination_unreachable(code);
	else
		printf("Bad ICMP Type: %d\n", type);
}

static void	ft_print_error(t_icmp_info *info, uint8_t flag)
{
	char				host_name[100];
	struct sockaddr_in	s_in;

	if (!info || info->rep.type == ICMP_ECHOREPLY
		|| info->rep.type == ICMP_ECHO)
		return ;
	s_in.sin_family = AF_INET;
	s_in.sin_port = 0;
	if (inet_pton(AF_INET, info->rep_src_addr, &(s_in.sin_addr)) == -1)
		return ;
	if (getnameinfo((struct sockaddr *)&(s_in),
			sizeof(s_in), host_name, 100, 0, 0, 0))
		return ;
	printf("%ld bytes from %s (%s): ", info->msg_size - sizeof(struct iphdr),
		host_name, info->rep_src_addr);
	ft_print_error_helper(info->rep.type, info->rep.code);
	if (info && (flag & FT_PING_VERBOSE))
	{
		ft_print_ip_hdr_dump(info->reply);
		ft_ping_iphdr((struct ip *)(info->reply));
		ft_ping_icmphdr((struct icmphdr *)(info->reply + FT_IP_HDR),
			info->msg_size - FT_IP_HDR - FT_ICMP_HDR);
	}
}

void	ft_print_helper(t_icmp_info *info, uint8_t flag)
{
	size_t	i;

	if (!info || (info->rep.type == ICMP_ECHO))
		return ;
	if (info->rep.type != ICMP_ECHOREPLY)
		return (ft_print_error(info, flag));
	if (flag | FT_PING_QUIET)
		return ;
	printf("%ld bytes from %s: icmp_seq=%d ",
		info->rep.data_size, info->rep_dst_addr, info->rep.seq);
	printf("ttl=%d time=%.3f ms\n", info->ttl, info->triptime);
	if (info->err_flags & FT_ERR_DUP)
		printf(" (DUP!)");
	if (info->err_flags & FT_INVALID_DSIZE)
		printf("\nWrong total length %ld instead of %ld",
			info->req.data_size + sizeof(struct icmphdr),
			info->msg_size - sizeof(struct ip));
	i = 0;
	while (i < info->req.data_size)
	{
		if (info->wrong_bytes[i])
			printf("\nwrong data byte #%ld should be %#x but was %#x",
				i, info->req.data[i], info->rep.data[i]);
		i += 1;
	}
}
