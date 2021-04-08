#include "ping.h"

static void	print_received(t_ping *ping, t_ping_pkt *pckt, long recv_bytes, char *recv_ip)
{
	double	time;
	char	*name;

	if (ping->fqdn)
		name = ping->fqdn;
	else
		name = ping->dest_name;
	if (pckt->hdr.type != ICMP_ECHOREPLY)
	{
		ft_printf("From %s (%s): icmp_seq=%d Time exceeded: Hop limit \n",
			name , recv_ip, ping->msg_count);
		return ;
	}
	time = ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec / 1000.0;
	time = time - (ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec / 1000.0);
	if (ping->rtt_min > time || ping->rtt_min == -1)
		ping->rtt_min = time;
	if (ping->rtt_max < time || ping->rtt_max == -1)
		ping->rtt_max = time;
	ping->rtt_sum += time;	
	ping->rtt_sum_sq += time * time;
	if (!ping->q)
			ft_printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
				recv_bytes, name, recv_ip, ping->msg_count, pckt->ip.ttl, time);
}

static void print_non_received(t_ping *ping, t_ping_pkt *pckt, long recv_bytes, char *recv_ip)
{
	char	*name;

	if (ping->fqdn)
		name = ping->fqdn;
	else
		name = ping->dest_name;
	if (!ping->q)
	{
		if (ping->v)
			ft_printf(" %ld bytes from %s (%s): type = %d, code = %d\n",
				recv_bytes, name, recv_ip, pckt->hdr.type, pckt->hdr.code);
		else
			ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n",
				recv_ip, ping->msg_count);
	}
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
	ping->fqdn = get_fqdn_info(ping->pr.sacrecv);
	recv_bytes = ret - sizeof(pckt->ip);
	if (!ping->q && ping->d)
		ft_printf("[%d.%d] ", ping->aft.tv_sec, ping->aft.tv_usec);
	if (ret <= 0 || pckt->hdr.code != 0)
		print_non_received(ping, pckt, recv_bytes, recv_ip);
	else
		print_received(ping, pckt, recv_bytes, recv_ip);
	if (ret > 0 && pckt->hdr.type == ICMP_ECHOREPLY)
		ping->msg_recv_count++;
	free(recv_ip);
	free(ping->fqdn);
}
