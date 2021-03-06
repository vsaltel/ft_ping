#include "ping.h"

int		g_state;

int		main(int argc, char **argv)
{
	t_ping	sping;

	g_state = 1;
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
	init_ping(&sping);
	get_args(&sping, argc, argv);
	if (sping.h)
		print_usage();
	else
		ping(&sping);
	free_args(&sping);
	return (0);
}
