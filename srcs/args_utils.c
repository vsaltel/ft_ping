#include "ping.h"

void	free_args(t_ping *ping)
{
	free(ping->dest_name);
	free(ping->dest_ip);
	free(ping->pr.sacrecv);
}

void	init_ping(t_ping *ping)
{
	ping->v = 0;
	ping->h = 0;
	ping->q = 0;
	ping->d = 0;
	ping->ttl = PING_TTL;
	ping->count_max = -1;
	ping->rtt_min = -1;
	ping->rtt_max = -1;
	ping->rtt_sum = 0;
	ping->rtt_sum_sq = 0;
	ping->msg_sent = 0;
	ping->msg_count = 0;
	ping->msg_recv_count = 0;
	ping->fqdn = NULL;
	ping->dest_name = NULL;
	ping->dest_ip = NULL;
	ping->info = NULL;
	ping->pid = getpid() & 0xffff;
	ping->datalen = PING_PKT_S;
	ping->pr.sasend = NULL;
	ping->pr.sacrecv = NULL;
	ping->pr.salen = 0;
}
