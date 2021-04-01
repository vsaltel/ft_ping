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

void		get_source_ip(t_ping *ping)
{
	struct ifaddrs	*id;
	int				val;

	val = getifaddrs(&id);
	if (!val)
		ft_strcpy(ping->src_ip, id->ifa_addr);
	printf("Network Interface Name :- %s\n",id->ifa_name);
	printf("Network Address of %s :- %d\n",id->ifa_name,id->ifa_addr);
	printf("Network Data :- %d \n",id->ifa_data);
	printf("Socket Data : -%c\n",id->ifa_addr->sa_data);
	freeifaddrs(id);
	return 0;
}

int			ping(t_ping *ping)
{
	struct addrinfo	*res;
	struct addrinfo	hints;
	int				sock;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(ping->dest_name, NULL, &hints, &res) != 0)
	{
		ft_dprintf(2, "ft_ping: %s: No address associated with hostname\n", ping->dest_name);
		return (1);
	}
	set_inetaddr(ping, res);
	freeaddrinfo(res);
	if ((sock = set_socket()) < 0)
		return (2);
	get_source_ip(ping);
	send_loop(ping, sock);
	return (0);
}
