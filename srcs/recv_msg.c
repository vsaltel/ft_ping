#include "ping.h"

void	recv_msg(t_ping *ping, t_ping_pkt *pckt)
{
	ssize_t				recv_bytes;
	char				*recv_ip;
	double				time;

	recv_bytes = recvfrom(ping->sockfd, pckt, sizeof(*pckt),
		0, ping->pr.sacrecv, &ping->pr.salen);
	gettimeofday(&ping->aft, NULL);
	recv_ip = set_inetaddr(ping->pr.sacrecv);
	if (recv_bytes <= 0 || pckt->hdr.code != 0)
		ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n", recv_ip, ping->msg_count);
	else
	{
		time = (ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec) / 1000.0;
		time = time - ((ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec) / 1000.0);
		if (ping->rtt_min > time || ping->rtt_min == -1)
			ping->rtt_min = time;
		if (ping->rtt_max < time || ping->rtt_max == -1)
			ping->rtt_max = time;
		ping->rtt_sum += time;	
		ping->rtt_sum_sq += time * time;
		ft_printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",  
			(int)recv_bytes, ping->dest_name, recv_ip, ping->msg_count, PING_TTL, time); 
		ping->msg_recv_count++;
	}
	free(recv_ip);
}
