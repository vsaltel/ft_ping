#include "ping.h"

static t_ping_pkt	set_pckt(t_ping *ping)
{
	t_ping_pkt			pckt;

	ft_bzero(&pckt, sizeof(pckt));
    pckt.hdr.type = ICMP_ECHO;
    pckt.hdr.un.echo.id = ping->pid;
    pckt.hdr.un.echo.sequence = ping->msg_count++;
    pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
	return (pckt);
}

int	read_loop(t_ping *ping)
{
	t_ping_pkt	pckt;

	if (!(ping->sockfd = set_socket(ping)))
		return (-5);
	ping->state = 1;
	catch_sigalrm(SIGALRM);	
	signal(SIGINT, &catch_sigint);
	while (ping->state)
	{
		//pckt = set_pckt(ping);
		recv_msg(ping, &pckt);
		struct timeval *tmp;
		tmp = (struct timeval *)&pckt.msg;
		ft_printf("recv usec %d\n", tmp->tv_usec);
	}
	print_final_stats(ping);
	return (0);
}

int	ping(t_ping *ping)
{
	struct addrinfo	*res;
	int				sock;
	int				ret;


	signal(SIGALRM, &catch_sigalrm);
	gettimeofday(&ping->launch_time, NULL);
	ping->bef = ping->launch_time;
	if (!(res = reverse_dns_info(ping->dest_name, NULL, AF_INET, 0)))
		return (-2);
	ping->dest_ip = set_inetaddr(res->ai_addr);
	ft_printf("PING %s (%s) %d data bytes\n", res->ai_canonname ? res->ai_canonname : ping->dest_name, ping->dest_ip, ping->datalen);
	if (res->ai_family != AF_INET)
		return (-3);
	ping->pr.sasend = res->ai_addr;
	ping->pr.sacrecv = malloc(res->ai_addrlen);
	ft_bzero(ping->pr.sacrecv, res->ai_addrlen);
	ping->pr.salen = res->ai_addrlen;
	if ((sock = set_socket(ping)) < 0)
		return (-4);
	ret = read_loop(ping);
	freeaddrinfo(res);
	return (ret);
}
