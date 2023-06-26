#include "ping.h"

static void	ft_ping_icmp_one_cadd(uint16_t *checksum, uint16_t buf)
{
	uint32_t	c;
	if (!checksum || !buf)
		return ;
	c = (*checksum) + buf;
	(*checksum) = (c & ((0x1 << 16) - 1));
	if (c & (0x1 << 16))
		ft_ping_icmp_one_cadd(checksum, 0x1);
}

void	ft_ping_icmp_checksum(uint8_t *buf, uint16_t *checksum, size_t bs)
{
	size_t		i;
	uint16_t	ret;
	uint16_t	ret1;

	if (!buf || !checksum || !bs)
		return ;
	ret1 = 0;
	while (i < bs)
	{
		ret = 0;
		if (i != bs - 1)
			ret = buf[i + 1];
		ft_ping_icmp_one_cadd(&ret1, (buf[i] << 8) | ret);
		i += 2;
	}
	ret1 = ~(ret1);
	*checksum = htons(ret1);
}

void	ft_ping_64bit_little_endian(uint8_t *msg, uint64_t val)
{
	int		i;
	uint8_t	*v;

	v = (uint8_t *)&val;
	if (BYTE_ORDER == BIG_ENDIAN)
		ft_memcpy(msg, v, sizeof(uint8_t) * 8);
	else
	{
		i = -1;
		while (++i < 8)
			msg[7 - i] = v[i];
	}
}

static int	ft_ping_add_timestamp(t_icmp *req_info, uint8_t *msg)
{
	if (!req_info)
		return (-1);
	if (gettimeofday(&(req_info->tv), 0) == -1)
		return (-1);
	ft_ping_64bit_little_endian((msg) + FT_PING_ICMP_HDR_SIZE
		, req_info->tv.tv_sec);
	ft_ping_64bit_little_endian((msg) + FT_PING_ICMP_HDR_SIZE
		+ sizeof(uint64_t), req_info->tv.tv_usec);
	ft_memcpy(req_info->data, msg + FT_PING_ICMP_HDR_SIZE, sizeof(uint64_t) * 2);
	return (0);
}

int	ft_ping_create_icmp_req(uint8_t **msg, t_icmp *req_info)
{
	size_t			p_size;
	struct icmphdr	*icmphdr;

	if (!req_info || !(req_info->data_size) || !msg)
		return (-1);
	p_size = req_info->data_size + FT_PING_ICMP_HDR_SIZE;
	if (!(req_info->data = (uint8_t *)malloc(sizeof(uint8_t) * req_info->data_size)))
		return (-1);
	if (!(*msg = (uint8_t *)malloc(sizeof(uint8_t) * p_size)))
		return (-1);
	icmphdr = (struct icmphdr *)(*msg);
	icmphdr->type = ICMP_ECHO;
	icmphdr->code = 0;
	icmphdr->un.echo.id = htons(req_info->id);
	icmphdr->un.echo.sequence = htons(req_info->seq);
	if (ft_ping_add_timestamp(req_info, *msg) == -1)
		return (-1);
	ft_ping_icmp_checksum(*msg, &(icmphdr->checksum), p_size);
	return (0);
}
