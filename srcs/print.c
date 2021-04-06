#include "ping.h"

void	print_final_stats(t_ping *ping)
{
	struct timeval end_time;
	long	rtt;

	gettimeofday(&end_time, NULL);
   	//tv_subtract(&end_time, &ping->launch_time);	 
	rtt = (end_time.tv_sec * 1000 + end_time.tv_usec) / 1000;
	rtt = rtt - ((ping->launch_time.tv_sec * 1000 + ping->launch_time.tv_usec) / 1000);
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %ld ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, rtt/* ping->total_stime / 1000*/);
}
