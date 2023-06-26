/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:28:39 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/26 18:53:21 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
# define ICMP_H

# include <stdint.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

# define FT_ICMP_HDR 8
# define FT_ICMP_DATA_SIZE 56

typedef struct s_icmp
{
	uint8_t			type;
	uint8_t			code;
	uint16_t		checksum;
	uint16_t		id;
	uint16_t		seq;
	size_t			data_size;

	struct timeval	tv;
	uint8_t			*data;
}					t_icmp;

typedef struct s_icmp_info
{
	t_icmp			req;
	t_icmp			res;
	char			src_addr[INET_ADDRSTRLEN];
	char			dst_addr[INET_ADDRSTRLEN];
	uint8_t			ttl;
	double			triptime;
}					t_icmp_info;

void				*ft_memcpy(void *dst, void *src, size_t n);
void				*ft_memset(void *b, int c, size_t n);

void				ft_ping_64bit_little_endian(uint8_t *msg, uint64_t val);
void				ft_icmp_cadd16(uint16_t *c1, uint16_t c2);
void				ft_icmp_checksum(uint8_t *buf,
						size_t buf_size, uint16_t *checksum);
int					ft_ping_create_icmp_req(uint8_t **msg, t_icmp *icmp);

#endif
