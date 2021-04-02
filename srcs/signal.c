#include "ping.h"

void	catch_sigint(int signal)
{
	(void)signal;
	g_ping.state = 0;
}

void	catch_sigalrm(int signal)
{
	if (signal != SIGALRM)
		return ;
	send_msg();	
	alarm(1);
}
