#include "ft_ping.h"

t_ping		*get_args(int ac, char **av)
{
	return (NULL);	
}

int			check_args(int ac, char **av)
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

void		print_usage(void)
{
	ft_printf("usage: ft_ping [-%s] host\n", OPTIONS);
}
