#include "ping.h"

int	main(int ac, char **av)
{
	uint8_t	*msg;
	t_icmp	req_info;

	if (ac != 2)
		return (1);
	(void)av;
	ft_memset(&req_info, 0, sizeof(t_icmp));
	msg = 0;
	req_info.id = 10;
	req_info.seq = 10;
	req_info.data_size = FT_PING_ICMP_DATA_SIZE;

	if (ft_ping_create_icmp_req(&msg, &req_info) == 0)
	{
		uint32_t	i = 0;
		while (i < FT_PING_ICMP_HDR_SIZE + req_info.data_size)
			printf("%#X ", msg[i++]);
		printf("\n");
	}
	if (msg)
		free(msg);
	if (req_info.data)
		free(req_info.data);
	return (0);
}
