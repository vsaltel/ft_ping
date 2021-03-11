#ifndef PING_H
# define PING_H

# define OPTIONS "hv"
# define PING_PKT_S 56
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

extern int			g_state;

typedef struct		s_ping
{
	int					h;	
	int					v;	
	int					msg_count;
	int					msg_recv_count;
	int					total_stime;
	char				*dest_name;
	char				dest_ip[INET6_ADDRSTRLEN];
	char				src_ip[INET6_ADDRSTRLEN];
	struct sockaddr_in	*sdest_v4;
	struct sockaddr_in6	*sdest_v6;
	struct sockaddr_in	sdest;
}					t_ping;

typedef struct		s_ping_pkt
{
	struct			icmphdr hdr;
	char			msg[PING_PKT_S-sizeof(struct icmphdr)];
}					t_ping_pkt;

int				ping(t_ping *ping);

void			init_ping(t_ping *ping);
void			get_args(t_ping *ping, int ac, char **av);
int				check_args(int ac, char **av);
void			free_args(t_ping *ping);

void			print_usage(void);
void			print_args(t_ping ping);

unsigned short	checksum(void *b, int len);

void			catch_sigint(int signal);

#endif
