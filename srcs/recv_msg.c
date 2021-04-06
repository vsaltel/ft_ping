#include "ping.h"

void	recv_msg(t_ping *ping, t_ping_pkt *pckt)
{
	ssize_t				recv_bytes;
	char				*recv_ip;
	double				rtt;

	recv_bytes = recvfrom(ping->sockfd, pckt, sizeof(*pckt),
		0, ping->pr.sacrecv, &ping->pr.salen);
	gettimeofday(&ping->aft, NULL);
	recv_ip = set_inetaddr(ping->pr.sacrecv);
	if (recv_bytes <= 0 || pckt->hdr.code != 0)
		ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n", recv_ip, ping->msg_count);
	else
	{
    	if ((ping->aft.tv_usec -= ping->def.tv_usec) < 0)
    	{
    	    --ping->aft.tv_sec;
    	    ping->aft.tv_usec += 1000000;
    	}
    	ping->aft.tv_sec -= ping->def.tv_sec;
		rtt = ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec / 1000.0;
		rtt = rtt - (ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec / 1000.0);
		ft_printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",  
			(int)recv_bytes, ping->dest_name, recv_ip, ping->msg_count, PING_TTL, rtt); 
		ping->msg_recv_count++;
	}
	free(recv_ip);
}
