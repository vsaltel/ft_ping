#include "ping.h"

void	print_final_stats(t_ping *ping)
{
	struct timeval end_time;
	double	rtt;

	gettimeofday(&end_time, NULL);
   	tv_substract(&end_time, &ping->launch_time);	 
	rtt = end_time.tv_sec * 1000.0 + end_time.tv_usec / 1000.0;
	rtt = rtt - (ping->launch_time.tv_sec * 1000.0 + ping->launch_time.tv_usec / 1000.0);
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %d ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, rtt/* ping->total_stime / 1000*/);
}
