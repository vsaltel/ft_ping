#include "ping.h"

void	catch_sigint(int signal)
{
	(void)signal;
	g_state = 0;
}
