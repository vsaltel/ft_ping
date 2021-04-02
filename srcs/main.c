#include "ping.h"

t_ping	g_ping;

int		main(int argc, char **argv)
{
	g_ping.state = 1;
	if (getuid() != 0)
	{
		ft_printf("ft_ping: need root privilege\n");
		return (1);
	}
	if (check_args(argc, argv))
	{
		print_usage();
		return (2);
	}
	init_ping(&g_ping);
	get_args(&g_ping, argc, argv);
	if (g_ping.h)
		print_usage();
	else
	{
		signal(SIGINT, &catch_sigint);
		signal(SIGALRM, &catch_sigalrm);
		ping(&g_ping);
	}
	free_args(&g_ping);
	return (0);
}
