#include "ping.h"

static void	set_pckt(t_ping *ping, t_ping_pkt *pckt)
{
	pckt->mhdr.msg_name = ping->pr.sacrecv;
	pckt->mhdr.msg_namelen = ping->pr.salen;
	pckt->mhdr.msg_iov = pckt->iov;
	pckt->mhdr.msg_iovlen = 1;
	pckt->mhdr.msg_control = &pckt->ctrl;
	pckt->mhdr.msg_controllen = sizeof(pckt->ctrl);
	pckt->iov[0].iov_base = pckt->databuf;
	pckt->iov[0].iov_len = sizeof(pckt->databuf);
}

static int	read_loop(t_ping *ping)
{
	t_ping_pkt	pckt;

	ping->sockfd = set_socket(ping);
	if (!ping->sockfd)
		return (-4);
	catch_sigalrm(SIGALRM);
	signal(SIGINT, &catch_sigint);
	ft_bzero(&pckt, sizeof(pckt));
	set_pckt(ping, &pckt);
	while (ping->count_max)
	{
		recv_msg(ping, &pckt);
		ping->msg_count++;
		ping->count_max--;
	}
	print_final_stats(ping);
	return (0);
}

static struct addrinfo	*get_addr_info(t_ping *ping)
{
	struct addrinfo	*info;

	info = reverse_dns_info(ping->dest_name, NULL, AF_INET, 0);
	if (!info)
		return (NULL);
	ping->dest_ip = set_inetaddr(info->ai_addr);
	ft_printf("FT_PING %s (%s) %d(%d) data bytes\n",
		info->ai_canonname ? info->ai_canonname : ping->dest_name,
		ping->dest_ip, ping->datalen,
		ping->datalen + sizeof(struct iphdr) + sizeof(struct icmphdr));
	if (info->ai_family != AF_INET)
		return (NULL);
	return (info);
}

int	ping(t_ping *ping)
{
	int				ret;

	signal(SIGALRM, &catch_sigalrm);
	gettimeofday(&ping->launch_time, NULL);
	ping->bef = ping->launch_time;
	ping->info = get_addr_info(ping);
	if (!ping->info)
		return (-2);
	ping->pr.sasend = ping->info->ai_addr;
	ping->pr.sacrecv = malloc(ping->info->ai_addrlen);
	ft_bzero(ping->pr.sacrecv, ping->info->ai_addrlen);
	ping->pr.salen = ping->info->ai_addrlen;
	ret = read_loop(ping);
	freeaddrinfo(ping->info);
	return (ret);
}
