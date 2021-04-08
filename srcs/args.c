#include "ping.h"

void			free_args(t_ping *ping)
{
	ft_printf("free %p\n", ping->dest_name);
	free(ping->dest_name);
}

void			get_args(t_ping *ping, int ac, char **av)
{
	int		n;
	int		x;

	n = 0;
	while (++n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			x = 1;
			if (av[n][x] == 't')
			{
				ping->ttl = ft_atoi(av[++n]);
				continue;
			}
			if (av[n][x] == 's')
			{
				ping->datalen = ft_atoi(av[++n]);
				continue;
			}
			if (av[n][x] == 'c')
			{
				ping->count_max = ft_atoi(av[++n]);
				continue;
			}
			if (av[n][x] == 'h')
				ping->h = 1;
			else if (av[n][x] == 'v')
				ping->v = 1;
			else if (av[n][x] == 'q')
				ping->q = 1;
			else if (av[n][x] == 'D')
				ping->d = 1;
		}
		else
			ping->dest_name = strdup(av[n]);
	}
}

int				check_args(int ac, char **av)
{
	int		n;
	int		x;
	int		dest;

	dest = 0;
	n = 0;
	while (++n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			x = 1;
			if (av[n][1] == 's' && av[n][2] == '\0')
			{
				if (++n == ac)
				{
					ft_printf("ft_ping: option requires an argument -- 's'\n");
					return (1);
				}
				else if (ft_atoi(av[n]) < 0 || ft_atoi(av[n]) > BUFSIZE)
				{
					ft_printf("ft_ping: illegal packet size\n");
					return (1);
				}
				continue;
			}
			if (av[n][1] == 't' && av[n][2] == '\0')
			{
				if (++n == ac)
				{
					ft_printf("ft_ping: option requires an argument -- 't'\n");
					return (1);
				}
				else if (ft_atoi(av[n]) <= 0 || ft_atoi(av[n]) > 255)
				{
					ft_printf("ft_ping: can't set unicast time-to-live: Invalid argument\n");
					return (1);
				}
				continue;
			}
			if (av[n][1] == 'c' && av[n][2] == '\0')
			{
				if (++n == ac)
				{
					ft_printf("ft_ping: option requires an argument -- 'c'\n");
					return (1);
				}
				else if (ft_atoi(av[n]) <= 0)
				{
					ft_printf("ft_ping: bad number of packets to transmit.\n");
					return (1);
				}
				continue;
			}
			while (av[n][x])
			{
				if (!strchr(OPTIONS, av[n][x]))
					return (1);
				x++;
			}
		}
		else if (++dest > 1)
			return (1);
	}
	if (!dest)
		return (1);
	return (0);
}

void			init_ping(t_ping *ping)
{
	ping->v = 0;
	ping->h = 0;
	ping->q = 0;
	ping->d = 0;
	ping->ttl = PING_TTL;
	ping->count_max = -1;
	ping->rtt_min = -1;
	ping->rtt_max = -1;
	ping->rtt_sum = 0;
	ping->rtt_sum_sq = 0;
	ping->msg_sent = 0;
	ping->msg_count = 0;
	ping->msg_recv_count = 0;
	ping->fqdn = NULL;
	ping->dest_name = NULL;
	ping->dest_ip = NULL;
	ping->pid = getpid() & 0xffff;
	ping->datalen = PING_PKT_S;
	ping->pr.sasend = NULL;
	ping->pr.sacrecv = NULL;
	ping->pr.salen = 0;
}

void			print_usage(void)
{
	ft_printf("usage: ft_ping [-%s] [-s packetsize] [-t ttl] host\n", OPTIONS);
}

void			print_args(t_ping ping)
{
	printf("DEST = %s\n", ping.dest_name);
	printf("OPTS = %s%s\n", ping.h ? "h" : "", ping.v ? "v" : "");
}
