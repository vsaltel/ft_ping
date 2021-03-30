#include "ping.h"

static void set_inetaddr(t_ping *ping, struct addrinfo *ai)
{
	void *addr;

	while (ai != NULL)
	{
		ping->sdest_v4 = (struct sockaddr_in *)ai->ai_addr;
		addr = &(ping->sdest_v4->sin_addr);
		if (!inet_ntop(ai->ai_family, addr, ping->dest_ip, sizeof(ping->dest_ip)))
			ft_strcpy(ping->dest_ip, "CONVERTION_FAIL");
		ai = ai->ai_next;
	}
	ft_printf("dest : %s\n", ping->dest_ip);
}

static int	set_socket(void)
{
	int				sock;
	int				ttl;
	struct timeval	tv_out;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		ft_dprintf(2, "ft_ping: fail to create socket\n");
		return (-1);
	}
	ttl = PING_TTL;
	if (setsockopt(sock, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
	{
		ft_dprintf(2, "ft_ping: fail to set ttl\n");
		return (-2);
	}
	tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0)
	{
		ft_dprintf(2, "ft_ping: fail to set timeout\n");
		return (-3);
	}
	return (sock);
}

int			ping(t_ping *ping)
{
	struct addrinfo	*res;
	struct addrinfo	hints;
	int				sock;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
	//hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(ping->dest_name, NULL, &hints, &res) != 0)
	{
		ft_dprintf(2, "ft_ping: %s: No address associated with hostname\n", ping->dest_name);
		return (1);
	}
	set_inetaddr(ping, res);
	freeaddrinfo(res);
	if ((sock = set_socket()) < 0)
		return (2);
	send_loop(ping, sock);
	return (0);
}
