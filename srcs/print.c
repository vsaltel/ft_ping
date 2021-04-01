#include "ping.h"

void	set_pckt(t_ping *ping, t_ping_pkt *pckt)
{
	int	i;

	bzero(pckt, sizeof(*pckt));
	pckt->hdr.type = ICMP_ECHO;
    pckt->hdr.un.echo.id = getpid();
	i = -1;
	while (++i < (int)sizeof(pckt->msg) - 1) 
        pckt->msg[i] = i + '0';
    pckt->msg[i] = 0;
    pckt->hdr.un.echo.sequence = ping->msg_count++;
    pckt->hdr.checksum = checksum(pckt, sizeof(*pckt));
}

void	print_final_stats(t_ping *ping)
{
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %d ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, ping->total_stime / 1000);
}

int	send_msg(t_ping *ping, int sock, t_ping_pkt *pckt)
{
	struct sockaddr		*ping_addr;
	int					ret;

	ping_addr = (struct sockaddr*)ping->sdest_v4;
	gettimeofday(&ping->bef, NULL);
	ret = sendto(sock, pckt, sizeof(*pckt), 0, ping_addr, sizeof(*ping_addr));
		char str[INET6_ADDRSTRLEN];
		struct sockaddr_in	*r_addr;
		r_addr = (struct sockaddr_in *)ping_addr;
		inet_ntop(AF_INET, &r_addr->sin_addr, str, INET6_ADDRSTRLEN);
		ft_printf("addr -> %s %d\n", str, r_addr->sin_addr);
	if (ret <= 0) 
	{ 
		ft_printf("Packet sending failed\n"); 
		return (0);
	}
	return (1);
}

void		get_source_ip(t_ping *ping, int sock)
{
struct sockaddr_in addr;
struct ifaddrs* ifaddr;
struct ifaddrs* ifa;
socklen_t addr_len;

addr_len = sizeof (addr);
getsockname(sock, (struct sockaddr*)&addr, &addr_len);
getifaddrs(&ifaddr);
for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
{
    if (ifa->ifa_addr)
    {
            struct sockaddr_in* inaddr = (struct sockaddr_in*)ifa->ifa_addr;

            if (inaddr->sin_addr.s_addr == addr.sin_addr.s_addr)
                if (ifa->ifa_name)
                {
					void	*addr;
					addr = &(inaddr->sin_addr.s_addr);
					if (!inet_ntop(inaddr->sin_family, addr, ping->src_ip, INET6_ADDRSTRLEN))
						ft_strcpy(ping->src_ip, "CONVERTION_FAIL");
					ft_printf("%s -> %s\n", ifa->ifa_name, ping->src_ip);
        		}
    }
}
freeifaddrs(ifaddr);
}

void	recv_msg(t_ping *ping, int sock, t_ping_pkt *pckt)
{
	ssize_t				recv_bytes;
	struct sockaddr_in	r_addr;
	socklen_t			addr_len;

	addr_len = sizeof(r_addr);
	recv_bytes = recvfrom(sock, pckt, sizeof(*pckt),
		0, (struct sockaddr *)&r_addr, &addr_len);
	gettimeofday(&ping->aft, NULL);
	if (ping->total_stime == -1)
		ping->total_stime = 0;
	else
		ping->total_stime = (ping->total_stime + (ping->aft.tv_usec - ping->bef.tv_usec));
	if (recv_bytes <= 0 || pckt->hdr.code != 0)
	{
		get_source_ip(ping, sock);
		ft_printf("From %s icmp_seq=%d Destination Host Unreachable\n", ping->src_ip, ping->msg_count);
	}
	else
	{
		ft_printf("pckt -> %d %d\n", pckt->hdr.type, pckt->hdr.code);
		ft_printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",  
			(int)recv_bytes, ping->dest_name, ping->dest_ip, ping->msg_count,
			PING_TTL, (float)(ping->aft.tv_usec - ping->bef.tv_usec) / 1000); 
		ping->msg_recv_count++;
	}
}

int	send_loop(t_ping *ping, int sock)
{
	t_ping_pkt			pckt;

	ft_printf("PING %s (%s) %d data bytes\n", ping->dest_name, ping->dest_ip, sizeof(t_ping_pkt));
	signal(2, &catch_sigint);
	while (g_state)
	{
		if (ping->msg_count)
			sleep(1);
 		set_pckt(ping, &pckt); 
		if (send_msg(ping, sock, &pckt))
			recv_msg(ping, sock, &pckt);
	}
	print_final_stats(ping);
	return (0);
}
