/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_icmp_echo_reply.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:45:41 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 15:14:02 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

static int	ft_icmp_init_echo_reply_hdr(uint8_t *msg, size_t msg_size,
				t_icmp *reply)
{
	if (!msg || !msg_size || !reply)
		return (-1);
	reply->type = msg[0];
	reply->code = msg[1];
	reply->checksum = ntohs(*((uint16_t *)(msg + 2)));
	reply->id = ntohs(*((uint16_t *)(msg + 4)));
	reply->seq = ntohs(*((uint16_t *)(msg + 6)));
	if (getimeofday(&(reply->tv), 0) < 0)
		return (-1);
	reply->data_size = msg_size - FT_ICMP_HDR;
	reply->data = (uint8_t *)malloc(sizeof(uint8_t) * reply->data_size);
	if (!reply->data)
		return (-1);
	ft_memcpy(reply->data, msg + FT_ICMP_HDR,
		sizeof(uint8_t) * reply->data_size);
	return (0);
}

static int	ft_icmp_init_echo_is_reply(t_icmp *reply)
{
	if (!reply || !(reply->data))
		return (-1);
	if (reply->type == ICMP_ECHO)
		return (1);
	if (reply->type == ICMP_ECHOREPLY)
		return (2);
	return (0);
}

int	ft_icmp_process_icmp_reply(uint8_t *msg, size_t msg_size, t_icmp *reply)
{
	int	ret;

	if (ft_icmp_ini_echo_reply_hdr(msg, msg_size, reply) == -1)
		return (-1);
	ret = ft_icmp_init_echo_is_reply(reply);
	if (ret < 2)
		return (ret);
}
