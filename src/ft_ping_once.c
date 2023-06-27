/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_once.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 17:49:03 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/27 18:02:53 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "icmp.h"

int	g_sig_handler = 0;

void	*signalhandler(int signo)
{
	g_sig_handler = signo;
	return (0);
}

static int	ft_recv_msg_helper(int sock_fd, struct msghdr *msghdr,
				t_icmp_stats *stats, t_icmp_info *info)
{
	ssize_t		bytes_recv;
	struct ip	*ip;

	if (!stats || !info || !msghdr || sock_fd < 0)
		return (-1);
	bytes_recv = recvmsg(sock_fd, msghdr, MSG_DONTWAIT);
	if (bytes_recv == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return (0);
	else if (bytes_recv == -1)
		return (-1);
	if (gettimeofday(&(info->rep.tv), 0) == -1)
		return (-1);
	ip = (struct ip *)(info->reply);
	inet_ntop(AF_INET, (void *)&(ip->ip_src), info->rep_src_addr,
		INET_ADDRSTRLEN);
	inet_ntop(AF_INET, (void *)&(ip->ip_dst), info->rep_dst_addr,
		INET_ADDRSTRLEN);
	info->ttl = ip->ip_ttl;
	return (ft_icmp_process_icmp_reply(stats, info, stats->flag));
}

static int	ft_recv_msg(int sock_fd, t_icmp_stats *stats, t_icmp_info *info)
{
	struct iovec	iov;
	struct msghdr	msghdr;
	int				ret;

	if (!stats || !info || sock_fd < 1)
		return (-1);
	iov.iov_base = (void *)(info->reply);
	iov.iov_len = FT_RECV_MSG;
	ft_memset(&msghdr, 0, sizeof(struct msghdr));
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;
	ret = 0;
	while (g_sig_handler != SIGALRM && g_sig_handler != SIGINT && !ret)
		ret = ft_recv_msg_helper(sock_fd, &msghdr, stats, info);
	if (ret == -1)
	{
		printf("recvmsg error : errno %d, message : %s\n",
			errno, strerror(errno));
		return (-1);
	}
	return (ret);
}

static int	ft_send_msg(int sock_fd, t_icmp_stats *stats, t_icmp_info *info)
{
	t_icmp	*req;
	uint8_t	*msg;
	ssize_t	nb;

	if (!stats || !info || !(info->req_addr))
		return (-1);
	req = &(info->req);
	req->id = stats->uid;
	req->seq = (stats->num_packets)++;
	req->data_size = FT_ICMP_DATA_SIZE;
	if (ft_ping_create_icmp_req(&msg, req) == -1)
		return (-1);
	nb = sendto(sock_fd, msg, FT_ICMP_DATA_SIZE + FT_ICMP_HDR, 0,
			info->req_addr, sizeof(struct sockaddr_in));
	free(msg);
	if (nb == -1 && errno == EACCES)
		printf("ft_ping: socket access error. Are you trying to ping"
			"broadcast ?\n");
	else if (nb == -1)
		printf("sendto err: %s\n", strerror(errno));
	if (nb == -1)
		return (-1);
	return (0);
}

int	ft_ping_once(int sock_fd, t_icmp_stats *stats, t_icmp_info *info)
{
	int	ret;

	if (!stats || !info || sock_fd < 0)
		return (-1);
	ft_memset(&(info->req), 0, sizeof(t_icmp));
	ft_memset(&(info->rep), 0, sizeof(t_icmp));
	ret = ft_send_msg(sock_fd, stats, info);
	if (!ret)
		alarm(1);
	if (!ret)
		ret = ft_recv_msg(sock_fd, stats, info);
	free(info->req.data);
	free(info->rep.data);
	ft_memset(&(info->req), 0, sizeof(t_icmp));
	ft_memset(&(info->rep), 0, sizeof(t_icmp));
	return (0);
}
