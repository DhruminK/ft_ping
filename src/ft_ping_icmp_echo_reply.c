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
	reply->data_size = msg_size - FT_ICMP_HDR;
	reply->data = (uint8_t *)malloc(sizeof(uint8_t) * reply->data_size);
	if (!(reply->data))
		return (-1);
	ft_memcpy(reply->data, msg + FT_ICMP_HDR,
		sizeof(uint8_t) * reply->data_size);
	return (0);
}

static void	ft_icmp_validate_data(t_icmp *req, t_icmp *rep, t_icmp_info *info)
{
	size_t	i;

	if (!req || !rep || !info)
		return ;
	ft_memset(info->wrong_bytes, 0, sizeof(req->data_size));
	i = 0;
	while (i < req->data_size && i < rep->data_size)
	{
		if (req->data[i] != rep->data[i])
			info->wrong_bytes[i] = 1;
		i += 1;
	}
}

static int	ft_icmp_validate_reply(t_icmp_info *info)
{
	uint8_t	*icmp;
	t_icmp	*reply;
	t_icmp	*req;

	reply = &(info->rep);
	icmp = (info->reply) + FT_IP_HDR;
	if (ft_icmp_init_echo_reply_hdr(icmp,
			info->msg_size - FT_IP_HDR, reply) == -1)
		return (-1);
	if (reply->type == ICMP_ECHO)
		return (1);
	if (reply->type != ICMP_ECHOREPLY)
		return (0);
	req = &(info->req);
	if (req->id != reply->id)
		return (1);
	info->err_flags = 0;
	if (req->seq != reply->seq)
		info->err_flags |= FT_ERR_DUP;
	if (req->data_size != reply->data_size)
		info->err_flags |= FT_INVALID_DSIZE;
	ft_icmp_validate_data(req, reply, info);
	return (0);
}

static void	ft_process_stats(t_icmp_stats *stats, t_icmp_info *info)
{
	t_icmp		*reply;
	uint64_t	req_time;
	uint64_t	rep_time;

	if (!stats || !info)
		return ;
	reply = &(info->rep);
	rep_time = (reply->tv.tv_sec * 1000 * 1000) + (reply->tv.tv_usec);
	req_time = (info->req.tv.tv_sec * 1000 * 1000) + (info->req.tv.tv_usec);
	info->triptime = (rep_time - req_time) / 1000;
	if (reply->type != ICMP_ECHOREPLY)
		return ;
	stats->num_packets_recv += 1;
	if (info->triptime < stats->t_min)
		stats->t_min = info->triptime;
	if (info->triptime > stats->t_max)
		stats->t_max = info->triptime;
	stats->t_sum += info->triptime;
	stats->vari += (info->triptime * info->triptime);
}

int	ft_icmp_process_icmp_reply(t_icmp_stats *stats,
		t_icmp_info *info, uint8_t flag)
{
	int	ret;

	if (!info || !stats)
		return (-1);
	ret = ft_icmp_validate_reply(info);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (0);
	ft_process_stats(stats, info);
	ft_print_helper(info, flag);
	return (1);
}
