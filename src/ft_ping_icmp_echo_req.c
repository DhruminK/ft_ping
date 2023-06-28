/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_icmp_echo_req.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:06:40 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 14:40:07 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

static int	ft_ping_add_timestamp(t_icmp *req, uint8_t *msg)
{
	if (!req || !msg)
		return (-1);
	if (gettimeofday(&(req->tv), 0) == -1)
		return (-1);
	ft_ping_64bit_little_endian((msg) + FT_ICMP_HDR, req->tv.tv_sec, sizeof(req->tv.tv_sec));
	ft_ping_64bit_little_endian((msg) + FT_ICMP_HDR + sizeof(req->tv.tv_sec),
		req->tv.tv_usec, sizeof(req->tv.tv_usec));
	ft_memcpy(req->data, msg + FT_ICMP_HDR, sizeof(uint64_t) * 2);
	return (0);
}

static void	ft_icmp_init_echo_hdr(uint8_t *msg, size_t size, t_icmp *req)
{
	uint16_t	*tmp;
	uint8_t		*data;
	size_t		n;

	if (!msg || !req)
		return ;
	msg[0] = ICMP_ECHO;
	msg[1] = 0;
	tmp = (uint16_t *)(msg + 2);
	*tmp = 0;
	tmp = (uint16_t *)(msg + 4);
	*tmp = htons(req->id);
	tmp = (uint16_t *)(msg + 6);
	*tmp = htons(req->seq);
	data = (msg + FT_ICMP_HDR);
	n = sizeof(struct timeval);
	while (n < size)
	{
		data[n] = n + 1 - sizeof(struct timeval);
		n += 1;
	}
}

int	ft_ping_create_icmp_req(uint8_t **msg, t_icmp *req)
{
	size_t		p_size;
	uint16_t	*checksum;

	if (!req || !msg || !(req->data_size))
		return (-1);
	p_size = FT_ICMP_HDR + req->data_size;
	req->data = (uint8_t *)malloc(sizeof(uint8_t) * req->data_size);
	if (!req->data)
		return (-1);
	*msg = (uint8_t *)malloc(sizeof(uint8_t) * p_size);
	if (!*msg)
		return (-1);
	ft_icmp_init_echo_hdr(*msg, req->data_size, req);
	ft_memcpy(req->data, *msg + FT_ICMP_HDR, sizeof(uint8_t) * req->data_size);
	if (ft_ping_add_timestamp(req, *msg) == -1)
		return (-1);
	checksum = (uint16_t *)((*msg) + 2);
	ft_icmp_checksum(*msg, p_size, checksum);
	return (0);
}
