#include "ping.h"

static int	check_options(t_ping *ping)
{
	if (ping->datalen > BUFSIZE)
		g_ping.datalen = BUFSIZE - sizeof(struct icmp) + sizeof(struct ip);
	if (!ping->dest_name)
		return (1);
	if (ping->ttl <= 0 || ping->ttl > 255)
	{
		ft_dprintf(2,
			"ft_ping: can't set unicast time-to-live: Invalid argument\n");
		return (1);
	}
	if (ping->datalen < 0)
	{
		ft_dprintf(2, "ft_ping: illegal packet size\n");
		return (1);
	}
	else if (ping->count_max <= 0 && ping->count_max != -1)
	{
		ft_dprintf(2, "ft_ping: bad number of packets to transmit.\n");
		return (1);
	}
	return (0);
}

static int	set_option(t_ping *ping, char **av,
	int *n, int x)
{
	if (av[*n][x] == 't')
		ping->ttl = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 's')
		ping->datalen = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 'c')
		ping->count_max = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 'h')
		ping->h = 1;
	else if (av[*n][x] == 'v')
		ping->v = 1;
	else if (av[*n][x] == 'q')
		ping->q = 1;
	else if (av[*n][x] == 'D')
		ping->d = 1;
	else
	{
		ft_dprintf(2, "ft_ping: invalid option -- '%c'\n", av[*n][x]);
		return (1);
	}
	return (0);
}

static int	set_option_loop(t_ping *ping, int ac,
	char **av, int *n)
{
	int	x;

	x = 0;
	while (av[*n][++x])
	{
		if (av[*n][x] == 't' || av[*n][x] == 's' || av[*n][x] == 'c')
		{
			if (av[*n][x + 1])
				return (1);
			if (*n + 1 == ac)
			{
				ft_dprintf(2, "ft_ping: option requires an argument -- '%c'\n",
					av[*n][x]);
				return (1);
			}
			return (set_option(ping, av, n, x));
		}
		if (set_option(ping, av, n, x))
			return (1);
	}
	return (0);
}

int	get_args(t_ping *ping, int ac, char **av)
{
	int	n;

	n = 0;
	while (++n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			if (set_option_loop(ping, ac, av, &n))
				return (1);
		}
		else if (ping->dest_name)
			return (1);
		else
			ping->dest_name = ft_strdup(av[n]);
	}
	if (check_options(ping))
		return (1);
	return (0);
}
