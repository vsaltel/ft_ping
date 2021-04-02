#include "ping.h"

struct ping_pkt	set_pckt(t_ping *ping)
{
	t_ping_pkt	pckt;
	int			i;

	ft_bzero(&pckt, sizeof(pckt));
    pckt.hdr.type = ICMP_ECHO;
    pckt.hdr.un.echo.id = ping->pid;
	i = 0;
    while (i < sizeof(pckt.msg) - 1)
	{
        pckt.msg[i] = i + '0';
		i++;
	}
    pckt.msg[i] = 0;
    pckt.hdr.un.echo.sequence = ping->msg_count++;
    pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
	return (pckt);
}

int	read_loop(t_ping *ping, int sock)
{
	struct ping_pkt	pckt;

	if (!(ping->sockfd = set_socket(ping)))
		return (1);
	catch_sigalrm(SIGALRM);	
	while (g_state)
	{
		pckt = set_pckt(ping);
		recv_msg(ping, &pckt);
	}
	print_final_stats(ping);
	return (0);
}

int			ping(t_ping *ping)
{
	struct addrinfo	*res;
	int				sock;
	int				ret;

	if (!(res = reverse_dns_info(ping->dest_name, NULL, AF_INET, 0))
		return (1);
	ping->dest_ip = set_inetaddr(ping, res);
	ft_printf("PING %s (%s) %d data bytes\n", res->ai_canonname ? host_addrinfo->ai_canonname : ping->dest_name, ping->dest_ip, ping->datalen);
	if (res->ai_family != AF_INET)
		return (2);
	if ((sock = set_socket()) < 0)
		return (3);
	ping->pr->sasend = res->ai_addr;
	ping->pr->sacrecv = ft_calloc(1, res->ai_addrlen);
	ping->pr->salen = res->ai_addrlen;
	ret = read_loop(ping, sock);
	freeaddrinfo(res);
	return (ret);
}
