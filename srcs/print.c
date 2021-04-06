#include "ping.h"

double	square_root(double a)
{
	double	s;
	double	l;
	double	mid;

	s = 1;
	l = a;
	mid = 0;
	while (s <= l)
	{
		mid = (s + l) / 2;
		if ((mid * mid) == a)
			break;
		if ((mid * mid) < a)
			s = mid + 1;
		else
			l = mid - 1;
	}
	mid = (s + l) / 2;
	return (mid);
}

void	print_final_stats(t_ping *ping)
{
	struct timeval	end_time;
	long			total_time;
	double			rtt_mdev;
	double			mean;
	double			smean;

	gettimeofday(&end_time, NULL);
	total_time = end_time.tv_sec * 1000 + end_time.tv_usec / 1000;
	total_time = total_time - (ping->launch_time.tv_sec * 1000 + ping->launch_time.tv_usec / 1000);
	ft_printf("--- %s ping statistics ---\n", ping->dest_name);
	ft_printf("%d packets transmitted, %d received, %d%% packet loss, time: %ld ms\n",
		ping->msg_count, ping->msg_recv_count,
		((ping->msg_count - ping->msg_recv_count)/ping->msg_count) * 100, total_time);
	if (ping->msg_recv_count)
	{
		mean = ping->rtt_sum / ping->msg_recv_count;
		smean = ping->rtt_sum_sq / ping->msg_recv_count;
		rtt_mdev = square_root(smean - (mean * mean));
		ft_printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			ping->rtt_min, mean, ping->rtt_max, rtt_mdev);
	}
}
