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

static void	ft_icmp_validate_data(t_icmp *req, t_icmp *rep, t_icmp_info *info)
{
	size_t	i;

	if (!req || !rep || !info)
		return (-1);
	ft_memset(info->wrong_bytes);
	i = 0;
	while (i < res->data_size && i < rep->data_size)
	{
		if (req->data[i] != rep->data[i])
			info->wrong_byte = 1;
		i += 1;
	}

}

static int	ft_icmp_validate_reply(t_icmp_info *info)
{
	uint8_t	*icmp;
	t_icmp	*reply;
	t_icmp	*req;

	reply = &(info->rep);
	*icmp = (info->msg_size) + FT_IP_HDR;
	if (ft_icmp_init_echo_reply_hdr(icmp,
			info->msg_size - FT_IP_HDR, reply) == -1)
		return (-1);
	if (info->type == ICMP_ECHO)
		return (1);
	if (info->type != ICMP_ECHOREPLY)
		return (0);
	req = &(info->req);
	if (req->id != res->id)
		return (1);
	info->err_flags = 0;
	if (req->seq != res->seq)
		info->err_flags |= FT_ERR_DUP;
	if (req->data_size != res->data_size)
		info->err_flags |= FT_INVALID_DSIZE;
	ft_icmp_validate_data(req, reply, info);
	return (0);
}

int	ft_icmp_process_icmp_reply(t_icmp_info *info, uint8_t flag)
{
	int	ret;

	if (!info)
		return (-1);
	ret = ft_icmp_validate_reply(info);
	if (ret == -1)
		return (-1);
	ft_print_helper(info, flag);
	return (0);
}
