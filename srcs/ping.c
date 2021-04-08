#include "ping.h"

int	read_loop(t_ping *ping)
{
	t_ping_pkt	pckt;

	if (!(ping->sockfd = set_socket(ping)))
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

	char buf[BUFSIZE];
	len = sizeof(struct sockaddr_in);
    if (getnameinfo(&res->ai_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
        printf("could not resolve hostname\n");
    else
		printf("host=%s\n", buf);
      

	ft_printf("FT_PING %s (%s) %d(%d) data bytes\n",
		res->ai_canonname ? res->ai_canonname : ping->dest_name,
		ping->dest_ip, ping->datalen, ping->datalen + sizeof(struct iphdr) + sizeof(struct icmphdr));
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
