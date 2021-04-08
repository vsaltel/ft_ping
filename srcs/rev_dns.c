#include "ping.h"

char	*get_fqdn_info(struct sockaddr *addr)
{
	char		buf[BUFSIZE];
	socklen_t	len;

	len = sizeof(struct sockaddr_in);
	if (getnameinfo(addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		return (NULL);
	return (ft_strdup(buf));
}

struct addrinfo	*reverse_dns_info(char *host, char *serv, int family,
	int socktype)
{
	struct addrinfo	*res;
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(host, serv, &hints, &res) != 0)
		ft_dprintf(2, "ft_ping: %s: No address associated with hostname\n",
			host);
	return (res);
}
