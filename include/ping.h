#ifndef PING_H
# define PING_H

# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <unistd.h>
# include <netinet/ip_icmp.h>
# include <stdlib.h>

# define FT_PING_ICMP_HDR_SIZE sizeof(struct icmphdr)
# define FT_PING_IP_HDR_SIZE sizeof(struct iphdr)
# define FT_PING_ICMP_DATA_SIZE 56

typedef struct s_icmp
{
	uint16_t		id;
	uint16_t		seq;
	size_t			data_size;

	struct timeval	tv;
	uint8_t			*data;
}					t_icmp;

int					ft_ping_parse_addr(char *buf, char dst_addr[INET_ADDRSTRLEN]);
int					ft_ping_create_icmp_req(uint8_t **msg, t_icmp *req_info);

void				*ft_memcpy(void *dst, void *src, size_t n);
void				*ft_memset(void *b, int c, size_t n);

#endif
