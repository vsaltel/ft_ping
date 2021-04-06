#include "ping.h"

static void		set_opt(t_ping *ping, char o)
{
	if (o == 'h')
		ping->h = 1;
	else if (o == 'v')
		ping->v = 1;
}

void			free_args(t_ping *ping)
{
	free(ping->dest_name);
}

void			get_args(t_ping *ping, int ac, char **av)
{
	int		n;
	int		x;

	n = 1;
	while (n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			x = 1;
			while (av[n][x])
			{
				set_opt(ping, av[n][x]);
				x++;
			}
		}
		else
			ping->dest_name = strdup(av[n]);
		n++;
	}
}

int				check_args(int ac, char **av)
{
	int		n;
	int		x;
	int		dest;

	dest = 0;
	n = 1;
	while (n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			x = 1;
			while (av[n][x])
			{
				if (!strchr(OPTIONS, av[n][x]))
					return (1);
				x++;
			}
		}
		else if (++dest > 1)
			return (1);
		n++;
	}
	if (!dest)
		return (1);
	return (0);
}

void			init_ping(t_ping *ping)
{
	ping->v = 0;
	ping->h = 0;
	ping->rtt_min = -1;
	ping->rtt_max = -1;
	ping->rtt_sum = 0;
	ping->rtt_sum_sq = 0;
	ping->msg_sent = 0;
	ping->msg_count = 0;
	ping->msg_recv_count = 0;
	ping->dest_name = NULL;
	ping->pid = getpid() & 0xffff;
	ping->sendbuf[0] = '\0';
	ping->pr.sasend = NULL;
	ping->pr.sacrecv = NULL;
	ping->pr.salen = 0;
}

void			print_usage(void)
{
	ft_printf("usage: ft_ping [-%s] host\n", OPTIONS);
}

void			print_args(t_ping ping)
{
	printf("DEST = %s\n", ping.dest_name);
	printf("OPTS = %s%s\n", ping.h ? "h" : "", ping.v ? "v" : "");
}
