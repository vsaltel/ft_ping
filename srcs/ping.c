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
			ping->sdest_v4 = (struct sockaddr_in *)ai->ai_addr;
			addr = &(ping->sdest_v4->sin_addr);
		}
		else // IPv6
		{ 
			ping->sdest_v6 = (struct sockaddr_in6 *)ai->ai_addr;
			addr = &(ping->sdest_v6->sin6_addr);
		}
		if (!inet_ntop(ai->ai_family, addr, ping->dest_ip, sizeof(ping->dest_ip)))
			ft_strcpy(ping->dest_ip, "CONVERTION_FAIL");
		ai = ai->ai_next;
	}
}

static void set_src_ip(t_ping *ping)
{
	struct addrinfo	*res;
	struct addrinfo	hints;
	void *addr;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
	if (getaddrinfo("127.0.0.1", NULL, &hints, &res) != 0)
	{
		ft_dprintf(2, "ft_ping: %s: No address associated with hostname\n", "127.0.0.1");
		return ;
	}
	while (res != NULL)
	{
		if (res->ai_family == AF_INET) // IPv4
			addr = &(ping->sdest_v4->sin_addr);
		else // IPv6
			addr = &(ping->sdest_v6->sin6_addr);
		printf("%d\n", (int)addr);
		if (!inet_ntop(res->ai_family, addr, ping->src_ip, sizeof(ping->src_ip)))
		res = res->ai_next;
	}
		ft_printf("my ip : %s\n", ping->src_ip);
}

static int	send_loop(t_ping *ping, int sock)
{
	int					flag;
	int					i;
	ssize_t				recv_bytes;
	socklen_t			addr_len;
	t_ping_pkt			pckt;
	struct sockaddr		*ping_addr;
	struct sockaddr_in	r_addr;
	struct timeval		bef;
	struct timeval		aft;

	ft_printf("PING %s (%s) %d data bytes\n", ping->dest_name, ping->dest_ip, sizeof(t_ping_pkt));
	flag = 1;
	signal(2, &catch_sigint);
	while (g_state)
	{
		if (ping->msg_count)
			sleep(1);
		bzero(&pckt, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
		i = -1;
		while (++i < (int)sizeof(pckt.msg) - 1) 
            pckt.msg[i] = i + '0';
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = ping->msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

		//send
		if (ping->sdest_v4)
			ping_addr = (struct sockaddr*)ping->sdest_v4;
		else
			ping_addr = (struct sockaddr*)ping->sdest_v6;
		gettimeofday(&bef, NULL);
		if (sendto(sock, &pckt, sizeof(pckt), 0, ping_addr, sizeof(*ping_addr)) <= 0) 
		{ 
			ft_printf("Packet sending failed\n"); 
			flag = 0;
		}

		//recv
		addr_len = sizeof(r_addr);
		if ((recv_bytes = recvfrom(sock, &pckt, sizeof(pckt), 0,
			(struct sockaddr *)&r_addr, &addr_len)) <= 0)
			ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n", ping->src_ip, ping->msg_count);
		gettimeofday(&aft, NULL);
		ping->total_stime = (ping->total_stime == -1 ? 0 : ping->total_stime + (aft.tv_usec - bef.tv_usec));
		if (recv_bytes > 0 && flag) 
		{ 
		/*
			if (!(pckt.hdr.type == 69 && pckt.hdr.code == 0))  
				printf("Error..Packet received with ICMP type %d code %d\n",  
					pckt.hdr.type, pckt.hdr.code); 
  			else
  			{ 
		*/
  				ft_printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",  
  					(int)recv_bytes, ping->dest_name, ping->dest_ip, ping->msg_count, PING_TTL, (float)(aft.tv_usec - bef.tv_usec) / 1000); 
  				ping->msg_recv_count++; 
  			//} 
  		}
	}
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %d ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, ping->total_stime / 1000);
	return (0);
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

	set_src_ip(ping);
	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
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
