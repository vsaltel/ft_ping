#include "ping.h"

unsigned short	checksum(void *b, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	result;

	buf = b;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

void	print_usage(void)
{
	ft_printf("usage: ft_ping [-%s] [-s packetsize] [-t ttl] host\n", OPTIONS);
}

void	print_args(t_ping ping)
{
	printf("DEST = %s\n", ping.dest_name);
	printf("OPTS = %s%s\n", ping.h ? "h" : "", ping.v ? "v" : "");
}
