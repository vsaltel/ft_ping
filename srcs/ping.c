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
		if (!inet_ntop(ai->ai_family, addr, ping->dest_ip, sizeof(ping->dest_ip)))
			ft_strcpy(ping->dest_ip, "CONVERTION_FAIL");
		ai = ai->ai_next;
	}
}

static int	send_loop(t_ping *ping, int sock)
{
	t_ping_pkt		pckt;

	g_state = 1;	
	signal(2, &catch_sigint);
	ft_printf("1\n");
	while (g_state)
	{
	/*
		bzero(&pckt, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        for ( i = 0; i < sizeof(pckt.msg)-1; i++ )
            pckt.msg[i] = i+'0';
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
	*/
	}
	ft_printf("2\n");
	return (0);
}

static int	set_socket(t_ping *ping)
{
	int				sock;
	int				ttl;
	struct timeval	tv_out;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		return (-1);
	ttl = 64;
	if (setsockopt(fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
	{
		ft_dprintf(2, "ft_ping: fail to set ttl\n");
		return (-2);
	}
	tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0)
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
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
	if (getaddrinfo(ping->dest_name, NULL, &hints, &res) != 0)
	{
		ft_dprintf(2, "ft_ping: cannot resolve %s: Unknown host\n", ping->dest_name);
		return (1);
	}
	set_inetaddr(ping, res);
	freeaddrinfo(res);
	ft_printf("IP: %s\n", ping->dest_ip);
	if ((sock = set_socket(ping)) < 0)
		return (2);
	send_loop(ping, sock);
	return (0);
}
