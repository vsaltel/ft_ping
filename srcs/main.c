#include "ping.h"

t_ping	g_ping;

int		main(int argc, char **argv)
{
	int	ret;

	ret = 0;
	if (getuid() != 0)
	{
		ft_printf("ft_ping: need root privilege\n");
		return (-1);
	}
	if (check_args(argc, argv))
	{
		print_usage();
		return (-1);
	}
	init_ping(&g_ping);
	get_args(&g_ping, argc, argv);
	if (g_ping.h)
		print_usage();
	else
		ret = ping(&g_ping);
	free_args(&g_ping);
	if (ret < 0)
		return (ret);
	if (g_ping.msg_recv_count)
		return (0);
	else if (g_ping.msg_sent)
		return (2);
	return (1);
}
