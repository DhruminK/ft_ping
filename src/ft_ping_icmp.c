#include "ping.h"

static void	ft_ping_icmp_one_cadd(uint16_t *checksum, uint16_t buf)
{
	uint32_t	c;

	if (!checksum || !buf)
		return ;
	c = (*checksum) + buf;
	(*checksum) = (c & 0xFF);
	if (c & (0x1 << 16))
		ft_ping_icmp_one_cadd(checksum, 0x1);
}

void	ft_ping_icmp_checksum(uint8_t *buf, uint8_t *checksum, size_t buf_size)
{
	size_t		i;
	uint16_t	ret;
	uint16_t	ret1;

	if (!buf || !checksum || !buf_size)
		return ;
	i = 0;
	ret1 = 0;
	while (i < buf_size)
	{
		ret = 0;
		if (i != buf_size - 1)
			ret = buf[i + 1];
		ft_ping_icmp_one_cadd(&ret1, (buf[i] << 8) | ret);
		i += 2;
	}
	ret1 = ~(ret1);
	checksum[0] = (ret1 >> 8) & 0xFF;
	checksum[1] = (ret1) & 0xFF;
}

static int	ft_ping_add_timestamp(uint8_t **msg, size_t data_size)
{
	struct timeval	tv;
	size_t			t_size;

	if (!msg || !*msg)
		return (-1);
	t_size = sizeof(struct timeval);
	if (t_size < data_size)
		t_size = data_size;
	if (gettimeofday(&tv, 0) == -1)
	{
		free(*msg);
		*msg = 0;
		return (-1);
	}
	ft_memcpy(*msg + FT_PING_ICMP_HDR_SIZE, &tv, sizeof(struct timeval));
	return (0);
}

int	ft_ping_create_icmp_req(uint8_t **msg, size_t data_size, t_icmp *icmp_info)
{
	struct icmphdr	*icmphdr;

	if (!data_size || !msg)
		return (-1);
	if (!(*msg = malloc(sizeof(uint8_t) * (data_size + FT_PING_ICMP_HDR_SIZE))))
		return (-1);
	icmphdr = (struct icmphdr *)(*msg);
	icmphdr->type = ICMP_ECHO;
	icmphdr->code = 0;
	icmphdr->un.echo.id = htons(icmp_info->id);
	icmphdr->un.echo.sequence = htons((icmp_info->seq)++);
	if (ft_ping_add_timestamp(msg, data_size) == -1)
		return (-1);
	icmphdr->checksum = 0;
	ft_ping_icmp_checksum(*msg, (uint8_t *)&(icmphdr->checksum), data_size + FT_PING_ICMP_HDR_SIZE);
	return (0);
}
