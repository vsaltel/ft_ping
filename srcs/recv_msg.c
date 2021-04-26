#include "ping.h"

static void	set_rtt(t_ping *ping, double time)
{
	if (ping->rtt_min > time || ping->rtt_min == -1)
		ping->rtt_min = time;
	if (ping->rtt_max < time || ping->rtt_max == -1)
		ping->rtt_max = time;
	ping->rtt_sum += time;
	ping->rtt_sum_sq += time * time;
}

static int	check_addr(char *addr)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (addr[i])
	{
		if (addr[i] == '.')
			n++;
		i++;
	}
	if (n > 1)
		return (0);
	return (1);
}

static void	print_received(t_ping *ping, t_ping_pkt *pckt,
	long recv_bytes, char *recv_ip)
{
	double	time;
	char	*name;

	name = ping->dest_name;
	if (check_addr(name) && ping->fqdn)
		name = ping->fqdn;
	if (pckt->icmp->icmp_type != ICMP_ECHOREPLY)
	{
		ft_printf("From %s (%s): icmp_seq=%d Time exceeded: Hop limit \n",
			name, recv_ip, ping->msg_count);
		return ;
	}
	time = ping->aft.tv_sec * 1000.0 + ping->aft.tv_usec / 1000.0;
	time = time - (ping->bef.tv_sec * 1000.0 + ping->bef.tv_usec / 1000.0);
	set_rtt(ping, time);
	if (!ping->q && ping->d)
		ft_printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d\n",
			recv_bytes, name, recv_ip, ping->msg_count, pckt->ip->ip_ttl);
	else if (!ping->q)
		ft_printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
			recv_bytes, name, recv_ip, ping->msg_count, pckt->ip->ip_ttl, time);
}

static void	print_non_received(t_ping *ping, t_ping_pkt *pckt,
	long recv_bytes, char *recv_ip)
{
	char	*name;

	name = ping->dest_name;
	if (check_addr(name) && ping->fqdn)
		name = ping->fqdn;
	if (!ping->q)
	{
		if (ping->v)
			ft_printf(" %ld bytes from %s (%s): type = %d, code = %d\n",
				recv_bytes, name, recv_ip, pckt->icmp->icmp_type,
				pckt->icmp->icmp_code);
		else
			ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n",
				recv_ip, ping->msg_count);
	}
}

void	recv_msg(t_ping *ping, t_ping_pkt *pckt)
{
	ssize_t	ret;
	char	*recv_ip;
	long	recv_bytes;

	ret = recvmsg(ping->sockfd, &pckt->mhdr, 0);
	pckt->ip = (struct ip *)pckt->databuf;
	pckt->icmp = (struct icmp *)(pckt->databuf + (pckt->ip->ip_hl << 2));
	gettimeofday(&ping->aft, NULL);
	recv_ip = set_inetaddr(ping->pr.sacrecv);
	ping->fqdn = get_fqdn_info(ping->pr.sacrecv);
	recv_bytes = ret - sizeof(struct ip);
	if (!ping->q && ping->d)
		ft_printf("[%d.%d] ", ping->aft.tv_sec, ping->aft.tv_usec);
	if (ret <= 0 || pckt->icmp->icmp_code != 0)
		print_non_received(ping, pckt, recv_bytes, recv_ip);
	else
		print_received(ping, pckt, recv_bytes, recv_ip);
	if (ret > 0 && pckt->icmp->icmp_type == ICMP_ECHOREPLY)
		ping->msg_recv_count++;
	free(recv_ip);
	free(ping->fqdn);
}
