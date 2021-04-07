#include "ping.h"

static void	print_received(t_ping *ping, t_ping_pkt *pckt, long recv_bytes, char *recv_ip)
{
	double	time;

	time = (ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec) / 1000.0;
	time = time - ((ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec) / 1000.0);
	if (ping->rtt_min > time || ping->rtt_min == -1)
		ping->rtt_min = time;
	if (ping->rtt_max < time || ping->rtt_max == -1)
		ping->rtt_max = time;
	ping->rtt_sum += time;	
	ping->rtt_sum_sq += time * time;
	ft_printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
		recv_bytes, ping->dest_name, recv_ip, ping->msg_count, pckt->ip.ttl, time);
	ping->msg_recv_count++;
}

void	recv_msg(t_ping *ping, t_ping_pkt *pckt)
{
	ssize_t	ret;
	char   	*recv_ip;
	long	recv_bytes;

	ret = recvfrom(ping->sockfd, pckt, sizeof(*pckt),
		0, ping->pr.sacrecv, &ping->pr.salen);
	gettimeofday(&ping->aft, NULL);
	recv_ip = set_inetaddr(ping->pr.sacrecv);
	recv_bytes = sizeof(pckt->hdr) + sizeof(pckt->msg);
	if (ret <= 0 || pckt->hdr.code != 0)
	{
		if (ping->v)
			ft_printf(" %ld bytes from %s (%s): type = %d, code = %d\n",
				recv_bytes, ping->dest_name, recv_ip, pckt->hdr.type, pckt->hdr.code);
		else
			ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n",
				recv_ip, ping->msg_count);
	}
	else
		print_received(ping, pckt, recv_bytes, recv_ip);
	free(recv_ip);
}
