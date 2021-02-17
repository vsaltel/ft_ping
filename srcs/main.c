#include "ping.h"

int		main(int argc, char **argv)
{
	t_ping	sping;

	if (check_args(argc, argv))
		print_usage();
	else
	{
		init_ping(&sping);
		get_args(&sping, argc, argv);
		print_args(sping);	
		ping(&sping);
		free_args(&sping);
	}
	return (0);
}
