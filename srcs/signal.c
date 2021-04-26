#include "ping.h"

void	catch_sigint(int signal)
{
	(void)signal;
	print_final_stats(ping);
	free_args(&g_ping);
	if (g_ping.msg_recv_count)
		exit(0);
	else if (g_ping.msg_sent)
		exit(2);
	exit(1);
}

void	catch_sigalrm(int signal)
{
	if (signal != SIGALRM)
		return ;
	send_msg();
	alarm(1);
}
