#include "ping.h"

//getpid.
//getuid.
//getaddrinfo.
//gettimeofday.
//inet_ntop.
//inet_pton.
//exit.
//signal.
//alarm.
//setsockopt.
//recvmsg.
//sendto.
//socket.

//inet_ntop(AF_INET, &(ping->si.sin_addr), str, INET_ADDRSTRLEN);

static void set_inetaddr(t_ping *ping, struct addrinfo *ai)
{
	void *addr;

	while (ai != NULL)
	{
		if (ai->ai_family == AF_INET) // IPv4
		{
			ping->ssrc_v4 = (struct sockaddr_in *)ai->ai_addr;
			addr = &(ping->ssrc_v4->sin_addr);
		}
		else // IPv6
		{ 
			ping->ssrc_v6 = (struct sockaddr_in6 *)ai->ai_addr;
			addr = &(ping->ssrc_v6->sin6_addr);
		}
		//ping->dest_ip = ft_strnew(INET6_ADDRSTRLEN);
		if (!inet_ntop(ai->ai_family, addr, ping->dest_ip, sizeof(ping->dest_ip)))
			ft_strcpy(ping->dest_ip, "CONVERTION_FAIL");
		ai = ai->ai_next;
	}
}

static int	set_soscket_options(int fd)
{
	int		on;

	on = 1;
	if (setsockopt(fd, SOL_SOCKET, IP_HDRINCL, &on, sizeof(on)) < 0)
		return (1);
	return (0);
}

static int	set_socket(t_ping *ping)
{
	int				sock;
	t_ping_pkt		pkt;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		return (1);
	return (0);
}

int			ping(t_ping *ping)
{
	struct addrinfo	*res;
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
	if (getaddrinfo(ping->dest_name, NULL, &hints, &res) != 0)
	{
		ft_dprintf(2, "ping: cannot resolve %s: Unknown host\n", ping->dest_name);
		return (1);
	}
	set_inetaddr(ping, res);
	freeaddrinfo(res);
	ft_printf("IP: %s\n", ping->dest_ip);
	set_socket(ping);
	return (0);
}
