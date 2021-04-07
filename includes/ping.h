#ifndef PING_H
# define PING_H

# define OPTIONS "hv"
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
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>

# include "libft.h"

typedef struct		s_proto
{
	struct sockaddr		*sasend;
	struct sockaddr		*sacrecv;
	socklen_t			salen;
}					t_proto;

typedef struct		s_ping
{
	int					state;
	int					h;	
	int					v;	
	int					q;	
	int					ttl;
	int					count_max;
	struct timeval		launch_time;
	struct timeval		bef;
	struct timeval		aft;
	double				rtt_min;
	double				rtt_max;
	double				rtt_sum;
	double				rtt_sum_sq;
	int					msg_sent;
	int					msg_count;
	int					msg_recv_count;
	char				*dest_name;
	char				*dest_ip;
	char				sendbuf[BUFSIZE];
	int					datalen;
	int					sockfd;
	t_proto				pr;
	pid_t				pid;
}					t_ping;

extern t_ping		g_ping;

typedef struct		s_ping_pkt
{
	struct			iphdr	ip;
	struct			icmphdr hdr;
	char			msg[BUFSIZE];
}					t_ping_pkt;

int				ping(t_ping *ping);
int				set_socket(t_ping *ping);
int				send_loop(t_ping *ping, int sock);
void			recv_msg(t_ping *ping, t_ping_pkt *pckt);
void			send_msg(void);

void			init_ping(t_ping *ping);
void			get_args(t_ping *ping, int ac, char **av);
int				check_args(int ac, char **av);
char			*set_inetaddr(struct sockaddr *sa);
void			free_args(t_ping *ping);
struct addrinfo	*reverse_dns_info(char *host, char *serv, int family, int socktype);

void			print_final_stats(t_ping *ping);
void			print_usage(void);
void			print_args(t_ping ping);

unsigned short	checksum(void *b, int len);

void			catch_sigint(int signal);
void			catch_sigalrm(int signal);

#endif
