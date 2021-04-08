#include "ping.h"

static int	read_loop(t_ping *ping)
{
	t_ping_pkt	pckt;

	ping->sockfd = set_socket(ping);
	if (!ping->sockfd)
		return (-5);
	ping->state = 1;
	catch_sigalrm(SIGALRM);
	signal(SIGINT, &catch_sigint);
	while (ping->state && ping->count_max)
	{
		ft_bzero(&pckt, sizeof(pckt));
		recv_msg(ping, &pckt);
		ping->msg_count++;
		ping->count_max--;
	}
	print_final_stats(ping);
	return (0);
}

static addrinfo	*get_addr_info(t_ping *ping)
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
}

int	ping(t_ping *ping)
{
	struct addrinfo	*info;
	int				sock;
	int				ret;

	signal(SIGALRM, &catch_sigalrm);
	gettimeofday(&ping->launch_time, NULL);
	ping->bef = ping->launch_time;
	info = get_addr_info(ping);
	if (!info)
		return (-2);
	ping->pr.sasend = info->ai_addr;
	ping->pr.sacrecv = malloc(info->ai_addrlen);
	ft_bzero(ping->pr.sacrecv, info->ai_addrlen);
	ping->pr.salen = info->ai_addrlen;
	sock = set_socket(ping);
	if (socket < 0)
		return (-4);
	ret = read_loop(ping);
	freeaddrinfo(info);
	return (ret);
}
