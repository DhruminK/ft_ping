/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 19:11:19 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 19:18:57 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

static void	ft_print_destination_unreachable(uint8_t code)
{
	if (code == 0)
		printf("Destination Net Unreachable\n");
	else if (code == 1)
		printf("Destination Host Unreachable\n");
	else if (code == 2)
		printf("Destination Protocol Unreachable\n");
	else if (code == 3)
		printf("Destination Port Unreachable\n");
	else if (code == 4)
		printf("Frag needed and DF set\n");
	else if (code == 5)
		printf("Source Route Failed\n");
	else
		printf("Destination Unreachable, Bad Code: %d\n", code);
}

static void	ft_print_redirect(uint8_t code)
{
	if (code == 0)
		printf("Redirect Network\n");
	else if (code == 1)
		printf("Redirect Host\n");
	else if (code == 2)
		printf("Redirect Type of Service and Network\n");
	else if (code == 3)
		printf("Redirect Type of Service and Host\n");
	else
		printf("Redirect, Bad Code: %d\n", code);
}

static void	ft_print_error_helper(uint8_t type, uint8_t code)
{
	if (type == 11 && code == 0)
		printf("Time of live exceeded\n");
	else if (type == 11 && code == 1)
		printf("Frag reassembly time exceeded\n");
	else if (type == 11)
		printf("Time exceeded, Bad Code: %d\n", code);
	else if (type == 12)
		printf("Parameter Problem\n");
	else if (type == 4)
		printf("Source Quench\n");
	else if (type == 13)
		printf("Timestamp\n");
	else if (type == 14)
		printf("Timestamp Reply\n");
	else if (type == 15)
		printf("Information Request\n");
	else if (type == 16)
		printf("Information Reply\n");
	else if (type == 5)
		ft_print_redirect(code);
	else if (type == 3)
		ft_print_destination_unreachable(code);
	else
		printf("Bad ICMP Type: %d\n", type);
}

static void	ft_print_error(char dst_addr[INET_ADDRSTRLEN], t_icmp *rep)
{
	if (!rep || (rep->type == 8) || (rep->type == 0))
		return ;
	printf("From %s icmp_seq=%d ", dst_addr, rep->seq);
	ft_print_error_helper(rep->type, rep->code);
}

void	ft_print_helper(t_icmp_info *info, uint8_t flag)
{
	if (!info || (info->res.type == 8))
		return ;
	if (info->res.type != 0)
		return (ft_print_error(info->dst_addr, &(info->res)));
	printf("%ld bytes from %s: icmp_seq=%d ",
		info->res.data_size, info->dst_addr, info->res.seq);
	if (flag & FT_PING_VERBOSE)
		printf("ident=%d ", getpid() & 0xFFFF);
	printf("ttl=%d time=%.3f ms\n", info->ttl, info->triptime);
}
