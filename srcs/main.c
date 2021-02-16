#include "ft_ping.h"

int main(int argc, char **argv)
{
	if (check_args(argc, argv))
		print_usage();
	return (0);
}
