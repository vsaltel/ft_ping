#include "ping.h"

void	send_msg(void)
{
	int					ret;
	int					len;
	struct	icmp		*icmp;

	icmp = (struct icmp *)g_ping.sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_ping.pid;
	icmp->icmp_seq = g_ping.nsent++;
	ft_memset(icmp->icmp_data, 0xa5, g_ping.datalen);
	gettimeofday((struct timeval *) icmp->icmp_data, NULL);
	len = 8 + g_ping.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
	ret = sendto(g_ping.sock, g_ping.sendbuf, len, 0, g_ping.pr.sasend, g_ping.pr.salen);
	if (ret <= 0) 
		ft_printf("Packet sending failed\n"); 
}