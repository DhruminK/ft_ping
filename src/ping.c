#include "ping.h"

int	main(int ac, char **av)
{
	char	dst_addr[INET_ADDRSTRLEN];
	char	*msg;
	int		ret;
	t_icmp	info;

	if (ac != 2)
		return (1);
	info.id = 10;
	info.seq = 10;
	ret = ft_ping_parse_addr(av[1], dst_addr);
	printf("RET : %d : %s (%d)\n", ret, (char *)dst_addr, INET_ADDRSTRLEN);
	if (ft_ping_create_icmp_req((uint8_t **)&msg, FT_PING_ICMP_DATA_SIZE, &info) == -1)
		return (-1);
	uint32_t	i = 0;
	while (i < FT_PING_ICMP_DATA_SIZE + sizeof(struct icmphdr))
		printf("%u ", msg[i++]);
	printf("\n");
	free(msg);
	return (0);
}
