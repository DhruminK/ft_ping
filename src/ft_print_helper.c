#include "ping.h"

void	ft_ping_iphdr(struct ip *ip)
{
	char	buf[INET_ADDRSTRLEN];
	char	*cp;
	size_t	hlen;

	if (!ip)
		return ;
	cp = (char *)ip + 20;
	hlen = ip->ip_hl << 2;
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro"
			"  cks      Src      Dst     Data\n");
	printf(" %x  %x  %02x %04x %04x",
			ip->ip_v, ip->ip_hl, ip->ip_tos,
			ntohs(ip->ip_len), ntohs(ip->ip_id));
	printf("   %1x %04lx",
			(uint32_t)(ntohs(ip->ip_off) & 0xe000) >> 13,
			(long unsigned int)(ntohs(ip->ip_off) & 0x1fff));
	printf("  %02x  %02x %04x", ip->ip_ttl, ip->ip_p,
			(ntohs(ip->ip_sum)));
	printf(" %s ", inet_ntop(AF_INET,
			&(ip->ip_src), buf, INET_ADDRSTRLEN));
	printf(" %s ", inet_ntop(AF_INET,
			&(ip->ip_dst), buf, INET_ADDRSTRLEN));
	while (hlen-- > 20)
		printf("%02x", *(cp++));
	printf("\n");
}

void	ft_ping_icmphdr(struct icmphdr *hdr, size_t data_size)
{
	if (!hdr)
		return ;
	printf("ICMP: type %d, code %d, size %ld",
		hdr->type, hdr->code, data_size);
	printf(", id %#x, seq %#x\n",
		ntohs(hdr->un.echo.id), ntohs(hdr->un.echo.sequence));
}

void	ft_print_ip_hdr_dump(uint8_t *iphdr)
{
	size_t	i;

	if (!iphdr)
		return ;
	printf("IP Hdr Dump:\n");
	i = 0;
	while (i < FT_IP_HDR)
	{
		if (!i && !(i % 2))
			printf(" ");
		printf("%x", iphdr[i]);
	}
	printf("\n");
}
