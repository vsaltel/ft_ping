#include "ping.h"

void	send_msg(void)
{
	int			len;
	int			ret;
	struct icmp	*icmp;
	struct ip	*ip;

	ip = (struct ip *)g_ping.sendbuf;
	ip->ip_hl = 5;
	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_len = sizeof(struct ip) + sizeof(struct icmp);
	ip->ip_id = g_ping.pid;
	ip->ip_off = 0;
	ip->ip_ttl = g_ping.ttl;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = checksum((u_short *) ip, len);
	ip->ip_dst = g_ping.pr.sasend; 

	icmp = (struct icmp *)(g_ping.sendbuf + sizeof(ip));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_ping.pid;
	icmp->icmp_seq = g_ping.msg_sent;
	ft_memset(icmp->icmp_data, 0xa5, g_ping.datalen);
	gettimeofday(&g_ping.bef, NULL);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + g_ping.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
	ret = sendto(g_ping.sockfd, g_ping.sendbuf, len, 0, g_ping.pr.sasend, g_ping.pr.salen);
	if (ret)
		g_ping.msg_sent++;
}
