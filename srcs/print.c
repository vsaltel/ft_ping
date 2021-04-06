#include "ping.h"
#include <math.h>

void	print_final_stats(t_ping *ping)
{
	struct timeval end_time;
	long	total_time;
	double	rtt_mdev;
	double	mean;	
	double	smean;	

	gettimeofday(&end_time, NULL);
	total_time = end_time.tv_sec * 1000 + end_time.tv_usec / 1000;
	total_time = total_time - (ping->launch_time.tv_sec * 1000 + ping->launch_time.tv_usec / 1000);
	mean = ping->rtt_sum / ping->msg_recv_count;
	smean = ping->rtt_sum_sq / ping->ping->msg_recv_count;
	rtt_mdev = sqrt(smean - (mean * mean));
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %ld ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, total_time);
	ft_printf("rtt min/avg/max/mdev = %3.f/%3.f/%3.f/%3.f ms",
		ping->rtt_min, mean, ping->rtt_max, rtt_mdev);
}
