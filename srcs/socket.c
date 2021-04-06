#include "ping.h"

int	set_socket(t_ping *ping)
{
	int	sock;
	int	size;

	if ((sock = socket(ping->pr.sasend->sa_family, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		ft_dprintf(2, "ft_ping: fail to create socket\n");
		return (-1);
	}
	size = 60 * 1024;
	setsockopt (sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	setsockopt(sock, IPPROTO_IP, IP_TTL, &ping->ttl, sizeof(ping->ttl));
	return (sock);
}
