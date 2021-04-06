#include "ping.h"

void	recv_msg(t_ping *ping, t_ping_pkt *pckt)
{
	ssize_t				recv_bytes;
	char				*recv_ip;
	double				rtt;

	recv_bytes = recvfrom(ping->sockfd, pckt, sizeof(*pckt),
		0, ping->pr.sacrecv, &ping->pr.salen);
	gettimeofday(&ping->aft, NULL);
	if (ping->total_stime == -1)
		ping->total_stime = 0;
	else
		ping->total_stime = (ping->total_stime + (ping->aft.tv_usec - ping->bef.tv_usec));
	recv_ip = set_inetaddr(ping->pr.sacrecv);
	if (recv_bytes <= 0 || pckt->hdr.code != 0)
		ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n", recv_ip, ping->msg_count);
	else
	{
		rtt = ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec / 1000.0;
		rtt = rtt - (ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec / 1000.0);
		ft_printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",  
			(int)recv_bytes, ping->dest_name, recv_ip, ping->msg_count,
			PING_TTL, rtt/*(float)(ping->aft.tv_usec - ping->bef.tv_usec) / 1000*/); 
		ping->msg_recv_count++;
	}
	free(recv_ip);
}
