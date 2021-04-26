#ifndef PING_H
# define PING_H

# define BUFSIZE 1500
# define PING_PKT_S 56
# define IP_STR_SIZE 129
# define PING_TTL 64
# define RECV_TIMEOUT 1

# include <stdio.h>
# include <signal.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in_systm.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>

# include "libft.h"

typedef struct s_ping_pkt
{
	struct msghdr	mhdr;
	struct iovec	iov[1];
	char			databuf[BUFSIZE];
	char			ctrl[BUFSIZE];
	struct ip		*ip;
	struct icmp		*icmp;
}				t_ping_pkt;

typedef struct s_proto
{
	struct sockaddr	*sasend;
	struct sockaddr	*sacrecv;
	socklen_t		salen;
}				t_proto;

typedef struct s_ping
{
	int				h;
	int				v;
	int				q;
	int				d;
	int				ttl;
	int				count_max;
	struct addrinfo	*info;
	struct timeval	launch_time;
	struct timeval	bef;
	struct timeval	aft;
	double			rtt_min;
	double			rtt_max;
	double			rtt_sum;
	double			rtt_sum_sq;
	int				msg_sent;
	int				msg_count;
	int				msg_recv_count;
	char			*fqdn;
	char			*dest_name;
	char			*dest_ip;
	int				datalen;
	int				sockfd;
	t_proto			pr;
	pid_t			pid;
}				t_ping;

extern t_ping	g_ping;

/*
** srcs/args.c
*/

int				get_args(t_ping *ping, int ac, char **av);

/*
** srcs/args_utils.c
*/

void			init_ping(t_ping *ping);
void			free_args(t_ping *ping);

/*
** srcs/conv_addr.c
*/

char			*set_inetaddr(struct sockaddr *sa);

/*
** srcs/ping.c
*/

int				ping(t_ping *ping);

/*
** srcs/recv_msg.c
*/

void			recv_msg(t_ping *ping, t_ping_pkt *pckt);

/*
** srcs/rev_dns.c
*/

struct addrinfo	*reverse_dns_info(char *host, char *serv,
					int family, int socktype);
char			*get_fqdn_info(struct sockaddr *addr);

/*
** srcs/send_msg.c
*/

void			send_msg(void);

/*
** srcs/socket.c
*/

int				set_socket(t_ping *ping);

/*
** srcs/print.c
*/

void			print_final_stats(t_ping *ping);

/*
** srcs/signal.c
*/

void			catch_sigint(int signal);
void			catch_sigalrm(int signal);

/*
** srcs/utils.c
*/

unsigned short	checksum(void *b, int len);
void			print_usage(void);

#endif
