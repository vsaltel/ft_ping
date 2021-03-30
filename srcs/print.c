#include "ping.h"

int	send_loop(t_ping *ping, int sock)
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
		ping_addr = (struct sockaddr*)ping->sdest_v4;
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
