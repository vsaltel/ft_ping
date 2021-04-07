#include "ping.h"

void	send_msg(void)
{
	int			len;
	int			ret;
	struct icmp	*icmp;

	icmp = (struct icmp *)g_ping.sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_ping.pid;
	icmp->icmp_seq = g_ping.msg_sent;
	ft_memset(icmp->icmp_data, 0xa5, g_ping.datalen);
	gettimeofday(&g_ping.bef, NULL);
	//icmp->icmp_data = malloc(50);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + g_ping.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
		ft_printf("send len %ld %d\n", sizeof(g_ping.sendbuf), len);
	ret = sendto(g_ping.sockfd, g_ping.sendbuf, len, 0, g_ping.pr.sasend, g_ping.pr.salen);
	if (ret)
		g_ping.msg_sent++;
}
