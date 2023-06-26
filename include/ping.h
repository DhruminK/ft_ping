/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:52:51 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 19:11:38 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_H
# define PING_H

# include <sys/time.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "icmp.h"

# define FT_PING_VERBOSE 0x1

int		ft_ping_parse_addr(char *buf, char dst_addr[INET_ADDRSTRLEN]);
void	ft_print_helper(t_icmp_info *info, uint8_t flag);

void	ft_ping_iphdr(struct ip *ip);
void	ft_ping_icmphdr(struct icmphdr *hdr, size_t data_size);
void	ft_print_ip_hdr_dump(uint8_t *iphdr);

#endif
