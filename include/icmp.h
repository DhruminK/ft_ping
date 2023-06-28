/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhatri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:28:39 by dkhatri           #+#    #+#             */
/*   Updated: 2023/06/27 18:00:52 by dkhatri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
# define ICMP_H

# include <errno.h>
# include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/time.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>

# define MAX_ITERATIONS 16

# define FT_ICMP_HDR 8
# define FT_ICMP_DATA_SIZE 56
# define FT_IP_HDR 20
# define FT_INVALID_DSIZE 0b1
# define FT_ERR_DUP 0b10
# define FT_IP_TTL 64
# define FT_RECV_MSG 200

# define FT_ARG_DOUBLE_HOST -2
# define FT_ARG_INV_EQUAL -3
# define FT_ARG_INVALID_ARG -4
# define FT_ARG_INVALID_ARG_WHOLE -5
# define FT_ARG_MISSING_HOST -6
# define FT_ROOT_PER_REQ 0
# define FT_UNKOWN_HOST 64

# define FT_PING_VERBOSE 0x1
# define FT_PING_QUIET 0x2
# define FT_PING_PACK_SIZE 0x4
# define FT_PING_TTL 0x6
# define FT_PING_HELP 0x8

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
	t_icmp			rep;
	uint8_t			reply[FT_RECV_MSG];
	uint8_t			wrong_bytes[FT_RECV_MSG];
	uint16_t		err_flags;
	size_t			msg_size;
	char			req_dst_addr[INET_ADDRSTRLEN];
	struct sockaddr	*req_addr;
	struct addrinfo	*res;
	struct addrinfo	*start_res;
	char			*arg;
	char			rep_src_addr[INET_ADDRSTRLEN];
	char			rep_dst_addr[INET_ADDRSTRLEN];
	uint8_t			ttl;
	double			triptime;
}					t_icmp_info;

typedef struct s_icmp_stats
{
	size_t			num_packets;
	size_t			num_packets_recv;
	char			*arg;
	double			t_min;
	double			t_sum;
	double			t_max;
	double			vari;
	uint16_t		uid;
	uint8_t			flag;
	uint8_t			ttl;
}					t_icmp_stats;

extern int			g_sig_handler;

void				*ft_memcpy(void *dst, void *src, size_t n);
void				*ft_memset(void *b, int c, size_t n);
int					ft_strcmp(char *s1, char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
size_t				ft_strlen(const char *s);
int					ft_atoi(char *str);

void				ft_ping_64bit_little_endian(uint8_t *msg, uint64_t val, uint8_t num_bytes);
void				ft_icmp_cadd16(uint16_t *c1, uint16_t c2);
void				ft_icmp_checksum(uint8_t *buf,
						size_t buf_size, uint16_t *checksum);
int					ft_ping_create_icmp_req(uint8_t **msg, t_icmp *icmp);
int					ft_icmp_process_icmp_reply(t_icmp_stats *stats,
						t_icmp_info *info, uint8_t flag);

void				ft_print_helper(t_icmp_info *info, uint8_t flag);

void				ft_ping_iphdr(struct ip *ip);
void				ft_ping_icmphdr(struct icmphdr *hdr, size_t data_size);
void				ft_print_ip_hdr_dump(uint8_t *iphdr);
void				ft_print_ping_info(t_icmp_info *info, uint8_t flag);
void				ft_print_stats(t_icmp_stats *stats);
int					ft_print_arg_error(int ret, char *val);

int					ft_newton(double val, uint16_t pow, double *out);
double				ft_pow(double val, int n);

int					ft_ping_once(int sock_fd, t_icmp_stats *stats,
						t_icmp_info *info);

int					ft_socket_init(t_icmp_stats *stats, t_icmp_info *info);

int					ft_ping_once(int sock_fd,
						t_icmp_stats *stats, t_icmp_info *info);

void				signalhandler(int signo);
int					ft_parse_args(int ac, char **av, t_icmp_stats *stats);

#endif
