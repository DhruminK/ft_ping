#include "ping.h"

static int	ft_ping_parse_addrinfo(struct addrinfo *res,
				char dst_addr[INET_ADDRSTRLEN])
{
	int					ret;
	struct	sockaddr_in	*s_addr;

	ret = 0;
	while (res)
	{
		ft_memset(dst_addr, 0, sizeof(char) * INET_ADDRSTRLEN);
		if (res->ai_family != AF_INET)
		{
			res = res->ai_next;
			continue ;
		}
		ret = 1;
		s_addr = (struct sockaddr_in *)res->ai_addr;
		if (!(inet_ntop(AF_INET, (void *)&(s_addr->sin_addr),
				dst_addr, INET_ADDRSTRLEN)))
			return (-1);
		break ;
	}
	return (ret);
}

static int	ft_ping_parse_host(char *buf, char dst_addr[INET_ADDRSTRLEN])
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	int				ret;

	if (!buf)
		return (-1);
	res = 0;
	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(buf, 0, &hints, &res) < 0)
		return (-1);
	ret = ft_ping_parse_addrinfo(res, dst_addr);
	freeaddrinfo(res);
	return (ret);
}

int	ft_ping_parse_addr(char *buf, char dst_addr[INET_ADDRSTRLEN])
{
	int					ret;

	if (!buf)
		return (-1);
	ret = inet_pton(AF_INET, buf, (void *)dst_addr);
	if (ret == 1)
		ft_memcpy(dst_addr, buf, sizeof(char) * INET_ADDRSTRLEN);
	if (ret == 0)
		ret = ft_ping_parse_host(buf, dst_addr);
	if (ret == -1 || ret == 0)
		return (-1);
	return (1);
}
